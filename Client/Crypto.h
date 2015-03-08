#pragma once

#include <aes.h>
#include <dh.h>
#include <modes.h>
#include <osrng.h>
#include <rsa.h>
#include <randpool.h>
#include <hex.h>
#include <rsa.h>
#include <osrng.h>
#include <base64.h>
#include <files.h>

#include <string>

#include "Connect.h"

class Crypto : public Connect
{
protected:

	CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption dec;

	CryptoPP::RSA::PrivateKey privateKey;
	CryptoPP::RSA::PublicKey publicKey;

	const char* Delimiter = "EOS";

public:

	virtual ~Crypto(){};

	//Delimiter�܂Ŏ�M����(����)
	std::string receiveEndOfStringsSynchro();

	//�������Delimiter��t���đ���(����)
	void sendEndOfStringsSynchro(std::string str);

	//Rsa�̌��J���𑗂�
	void sendRsaPublicKey();

	//Rsa�̈Í�����ǂ���
	std::string decryptionByRsa(std::string str);

	//AES�̈Í�����ǂ���
	std::string decryptionByAes(std::string str);

	//AES�̋��ʌ����󂯎��
	void receiveAESKey();

	//Rsa�ňÍ�������
	std::string encryptByRsa(std::string strShortString);

	//AES�ňÍ�������
	std::string encryptByAes(std::string plainText);
};

