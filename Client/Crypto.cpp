#include "Crypto.h"

#include <boost/asio.hpp>

#include <bitset>
#include <random>

namespace asio = boost::asio;

std::string Crypto::receiveEndOfStringsSynchro()
{
	asio::streambuf receive_buffer;
	receive_buffer.consume(receive_buffer.size());

L1:
	boost::system::error_code error;
	asio::read_until(socket, receive_buffer, Delimiter, error);

	std::string buffer(std::string(boost::asio::buffer_cast<const char*>(receive_buffer.data()), receive_buffer.size()));
	auto length = buffer.find(Delimiter);

	if (length != std::string::npos)
	{
		return buffer.substr(0, length);
	}
	else
	{
		receive_buffer.consume(length + strlen(Delimiter));

		goto L1;
	}
}

void Crypto::sendEndOfStringsSynchro(std::string str)
{
	boost::system::error_code error;
	asio::streambuf send_buffer;

	std::ostream os(&send_buffer);
	os << str << Delimiter;

	write(socket, send_buffer.data(), error);
	send_buffer.consume(send_buffer.size());
}

void Crypto::sendRsaPublicKey()
{
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::InvertibleRSAFunction params;

	//RSA�L�[�̐���
	params.GenerateRandomWithKeySize(rng, 3072);

	privateKey = CryptoPP::RSA::PrivateKey(params);
	publicKey = CryptoPP::RSA::PublicKey(params);

	//publicKey��ۑ�
	CryptoPP::RSAFunction pubkey(params);

	//pubkey��ۑ�
	std::string pubkeyfile;

	CryptoPP::Base64Encoder pubkeysink(new CryptoPP::StringSink(pubkeyfile));
	pubkey.DEREncode(pubkeysink);
	pubkeysink.MessageEnd();

	//rsa public�L�[�𑗂�
	sendEndOfStringsSynchro("c" + pubkeyfile);
}

std::string Crypto::decryptionByRsa(std::string str)
{
	std::string recovered;
	// Decryption
	CryptoPP::RSAES_OAEP_SHA_Decryptor d(privateKey);
	CryptoPP::AutoSeededRandomPool rndg;

	CryptoPP::StringSource ss2(str, true,
		new CryptoPP::PK_DecryptorFilter(rndg, d,
		new CryptoPP::StringSink(recovered)
		) // PK_DecryptorFilter
		); // StringSource

	return recovered;
}

std::string Crypto::decryptionByAes(std::string str)
{
	// �������̂��߂̕ϊ��t�B���^�̍쐬
	std::string decText;
	CryptoPP::StreamTransformationFilter decFilter(dec, new CryptoPP::StringSink(decText));
	decFilter.Put(reinterpret_cast<const byte*>(str.c_str()), str.size());
	//���܂��Ȃ�
	decFilter.MessageEnd();

	return decText;
}

void Crypto::receiveAESKey()
{
	//���̐���
	byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
	byte iv[CryptoPP::AES::BLOCKSIZE];

	//�����̃V�[�h���󂯎��
	std::string restr = receiveEndOfStringsSynchro();

	std::string buffer = decryptionByRsa(restr);

	if (buffer != "0") {

		char *e;//�ϊ��s�\
		size_t randomseed = std::strtoul(buffer.substr(0, buffer.size()).c_str(), &e, 10);

		//�����Z���k�E�c�C�X�^
		std::mt19937 randomEngine(randomseed);
		std::uniform_int_distribution<int> randomIntDistribution(-128, 127);

		//���ʌ���IV��������
		size_t k = 0, v = 0, kd = 0, vd = 0;
		while (kd + vd != 2)
		{
			std::string tes = std::bitset<8>(randomIntDistribution(randomEngine)).to_string();

			if (tes[5] != 0)randomIntDistribution(randomEngine);

			if (tes[7] == '1' || tes[4] == '0')
			{
				int8_t r = randomIntDistribution(randomEngine);
				key[k] = r;

				k++;
				if (k == sizeof(key))k = 0, kd = 1;
			}
			else
			{
				int8_t r = randomIntDistribution(randomEngine);
				iv[v] = r;

				v++;
				if (v == sizeof(iv))v = 0, vd = 1;
			}
		}
		// �������I�u�W�F�N�g�̍쐬
		dec.SetKeyWithIV(key, sizeof(key), iv);
	}
}

std::string Crypto::encryptByRsa(std::string strShortString)
{
	CryptoPP::AutoSeededRandomPool rnd;

	std::string cipher;
	// Encryption
	CryptoPP::RSAES_OAEP_SHA_Encryptor e(publicKey);

	CryptoPP::StringSource ss1(strShortString, true,
		new CryptoPP::PK_EncryptorFilter(rnd, e,
		new CryptoPP::StringSink(cipher)
		) // PK_EncryptorFilter
		); // StringSource

	return cipher;
}

std::string Crypto::encryptByAes(std::string plainText)
{
	// �Í����̂��߂̕ϊ��t�B���^�̍쐬
	std::string encText;
	CryptoPP::StreamTransformationFilter encFilter(dec, new CryptoPP::StringSink(encText));

	// �Í���
	encFilter.Put(reinterpret_cast<const byte*>(plainText.c_str()), plainText.size());
	encFilter.MessageEnd();

	return  encText;
}