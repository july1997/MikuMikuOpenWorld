#include "Network.h"

Network::Network()
{
}


Network::~Network()
{
}

int Network::connect(std::string Ip)
{
	IPDATA IP;

	for (int i = 0; i < 4; i++) {
		switch (i)
		{
		case 0:
			IP.d1 = stoi(Ip.substr(0, Ip.find(".")));
			Ip.erase(0, Ip.find(".") + 1);
			break;
		case 1:
			IP.d2 = stoi(Ip.substr(0, Ip.find(".")));
			Ip.erase(0, Ip.find(".") + 1);
			break;
		case 2:
			IP.d3 = stoi(Ip.substr(0, Ip.find(".")));
			Ip.erase(0, Ip.find(".") + 1);
			break;
		case 3:
			IP.d4 = stoi(Ip.c_str());
			break;
		}
	}

	nethandle = ConnectNetWork(IP, 13939);

	return nethandle;
}

int Network::send(char command, std::string method, std::string str, bool changeUTF8,bool base64encode, bool useRSA, bool useAES)
{
	char end = 0;
	string strs = str;

	//if (changeUTF8)strs = convertUTF8(strs);

	if (useRSA)strs = encryptByRsa(strs);

	if (useAES)encryptByAes(strs);

	string strn = string(command + method + " " + strs);
	
	//base64encod
	string encoded;
	CryptoPP::StringSource ss((const byte*)strn.data(), strn.size(), true,
		new CryptoPP::Base64Encoder(
			new CryptoPP::StringSink(encoded)
		) // HexEncoder
	); // StringSource
	encoded = encoded + end;

	if (encoded.size() >= 512) {
		NetWorkSend(nethandle, encoded.substr(0,512).c_str(), 512);
		encoded.erase(0, 512);
	}

	return NetWorkSend(nethandle, encoded.c_str(), encoded.size());
}


int Network::receive_start()
{
	t = std::thread(&Network::receiveLoop, this);
	t.detach();

	return 0;
}

int Network::receiveLoop()
{
	//別スレッドで立ち上げ

	while (!ProcessMessage())
	{
		// 切断確認
		if (GetLostNetWork() == nethandle)
		{
			message.push_back("SYSTEM サーバーから切断されました");
			break;
		}

		// 受信した文字列がある場合は受信する
		if (GetNetWorkDataLength(nethandle) > 0)
		{
			char Message[2048], Message2[2048];
			for (int i = 0; i < 2048; i++)
			{
				Message[i] = 0;
				Message2[i] = 0;
			}
			NetWorkRecvToPeek(nethandle, Message, 2047);

			std::string buf(Message);

			// メッセージを受信
			NetWorkRecv(nethandle, Message2, buf.size()+1);

			std::string str(Message2);

			string decoded;
			CryptoPP::StringSource ssk(str, true /*pump all*/,
				new CryptoPP::Base64Decoder(
					new CryptoPP::StringSink(decoded)
				) // HexDecoder
			); // StringSource

			if (str.size() > 1)
			{
				message.push_back(decoded);
			}
		}
	}

	return 0;
}

size_t Network::getMassegeSize()
{
	return message.size();
}

std::string Network::getMassege()
{
	string m = message[0];
	message.erase(message.begin());

	return m;
}

int Network::makeUDPSocket(std::string Ip)
{
	// ＵＤＰ通信用のソケットハンドルを作成
	netudphandle = MakeUDPSocket(-1);

	for (int i = 0; i < 4; i++) {
		switch (i)
		{
		case 0:
			UDPIP.d1 = stoi(Ip.substr(0, Ip.find(".")));
			Ip.erase(0, Ip.find(".")+1);
			break;
		case 1:
			UDPIP.d2 = stoi(Ip.substr(0, Ip.find(".")));
			Ip.erase(0, Ip.find(".")+1);
			break;
		case 2:
			UDPIP.d3 = stoi(Ip.substr(0, Ip.find(".")));
			Ip.erase(0, Ip.find(".")+1);
			break;
		case 3:
			UDPIP.d4 = stoi(Ip.c_str());
			break;
		}
	}

	return 0;
}

void Network::sendUDP(char command, std::string method, string Buffer, bool base64encode, bool useAES)
{
	char end = 0;
	string strs = Buffer;

	if (useAES)encryptByAes(strs);

	string strn = string(command + method + " " + strs);
	//base64encod
	string encoded;
	CryptoPP::StringSource ss((const byte*)strn.data(), strn.size(), true,
		new CryptoPP::Base64Encoder(
			new CryptoPP::StringSink(encoded)
		) // HexEncoder
	); // StringSource
	encoded = encoded + end;

	// 文字列の送信
	NetWorkSendUDP(netudphandle, UDPIP, 23939, encoded.c_str(), encoded.size());
}

int Network::UDP_receive_start()
{
	recivenetudphandle = MakeUDPSocket(23940);

	u = std::thread(&Network::UDP_receiveLoop, this);
	u.detach();

	return 0;
}

int Network::UDP_receiveLoop()
{
	//別スレッドで立ち上げ

	while (!ProcessMessage())
	{
		if (CheckNetWorkRecvUDP(recivenetudphandle) == TRUE)
		{
			char StrBuf[256], StrBuf2[256];
			for (int i = 0; i < 256; i++)
			{
				StrBuf[i] = 0;
				StrBuf2[i] = 0;
			}

			NetWorkRecvUDP(recivenetudphandle, NULL, NULL, StrBuf, 256, TRUE);
			std::string buf(StrBuf2);

			// 受信
			NetWorkRecvUDP(recivenetudphandle, NULL, NULL, StrBuf, buf.size()+1, FALSE);

			std::string str(StrBuf);

			string decoded;
			CryptoPP::StringSource ssk(str, true /*pump all*/,
				new CryptoPP::Base64Decoder(
					new CryptoPP::StringSink(decoded)
				) // HexDecoder
			); // StringSource
			command.push_back(decoded);
		}
	}

	return 0;
}

size_t Network::getCommandSize()
{
	return command.size();
}

std::string Network::getCommand()
{
	string m = command[0];
	command.erase(command.begin());

	return m;
}

int Network::getRsaPublicKey(std::string publicKeystr)
{
	try
	{
		// Use the array directly to avoid the copy
		CryptoPP::ArraySource as(publicKeystr, true /*pumpAll*/);

		// Use Load to BER decode the Subject Public Key Info (SPKI)
		pubkey.Load(as);

		// Validate it before using it
		CryptoPP::AutoSeededRandomPool prng;
		pubkey.ThrowIfInvalid(prng,1);
	}
	catch (CryptoPP::Exception const& e)
	{
		return -1;
	}
	catch (...)
	{
		return -1;
	}

	return 0;
}

bool Network::signature(string str)
{
	try
	{
		string recovered;

		// Verify and Recover
		CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA1>::Verifier verifier(pubkey);

		CryptoPP::StringSource ss2(version + str, true,
			new CryptoPP::SignatureVerificationFilter(
				verifier,
				new CryptoPP::StringSink(recovered),
				CryptoPP::SignatureVerificationFilter::THROW_EXCEPTION |
				CryptoPP::SignatureVerificationFilter::PUT_MESSAGE
			) // SignatureVerificationFilter
		); // StringSource


		if(recovered == version)return 1;
		else return 0;
	}
	catch (...)
	{
		return 0;
	}
}

std::string Network::encryptByRsa(std::string str)
{
	CryptoPP::AutoSeededRandomPool rnd;

	std::string cipher;
	// Encryption
	CryptoPP::RSAES_OAEP_SHA_Encryptor e(pubkey);

	CryptoPP::StringSource ss1(str, true,
		new CryptoPP::PK_EncryptorFilter(rnd, e,
			new CryptoPP::StringSink(cipher)
		) // PK_EncryptorFilter
	); // StringSource

	return cipher;
}


int Network::makeAESKey()
{
	byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
	byte iv[CryptoPP::AES::BLOCKSIZE];

	while (1) {

		// 共通鍵とIVを適当な値で初期化
		CryptoPP::AutoSeededRandomPool prng;
		prng.GenerateBlock(key, sizeof(key));
		prng.GenerateBlock(iv, CryptoPP::AES::BLOCKSIZE);

		common_key_ = std::string((const char*)key, sizeof(key));
		common_key_iv_ = std::string((const char*)iv, sizeof(iv));

		if (common_key_.size() == 16 && common_key_iv_.size() == 16 &&
			common_key_.find(" ") == std::string::npos && common_key_iv_.find(" ") == std::string::npos)break;
	}

	WaitTimer(1000);

	send(2,"AES", common_key_ + " " + common_key_iv_,0,1,1);

	enc.SetKeyWithIV(key, sizeof(key), iv);
	dec.SetKeyWithIV(key, sizeof(key), iv);


	return 0;
}

//暗号化
void Network::encryptByAes(std::string &str)
{
	enc.ProcessData((byte*)str.data(), (byte*)str.data(), str.size());
}

//復号化
void Network::decryptionByAes(std::string &str)
{
	dec.ProcessData((byte*)str.data(), (byte*)str.data(), str.size());
}

std::string Network::convertShiftJIS(const std::string srcUTF8)
{
	//Unicodeへ変換後の文字列長を得る
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, NULL, 0);

	//必要な分だけUnicode文字列のバッファを確保
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//UTF8からUnicodeへ変換
	MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode);

	//ShiftJISへ変換後の文字列長を得る
	int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

	//必要な分だけShiftJIS文字列のバッファを確保
	char* bufShiftJis = new char[lengthSJis];

	//UnicodeからShiftJISへ変換
	WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);

	std::string strSJis(bufShiftJis);

	delete bufUnicode;
	delete bufShiftJis;

	return strSJis;
}

std::string Network::convertUTF8(std::string srcShiftJIS)
{
	//Unicodeへ変換後の文字列長を得る
	int lenghtUnicode = MultiByteToWideChar(CP_THREAD_ACP, 0, srcShiftJIS.c_str(), srcShiftJIS.size() + 1, NULL, 0);

	//必要な分だけUnicode文字列のバッファを確保
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//ShiftJISからUnicodeへ変換
	MultiByteToWideChar(CP_THREAD_ACP, 0, srcShiftJIS.c_str(), srcShiftJIS.size() + 1, bufUnicode, lenghtUnicode);


	//UTF8へ変換後の文字列長を得る
	int lengthUTF8 = WideCharToMultiByte(CP_UTF8, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

	//必要な分だけUTF8文字列のバッファを確保
	char* bufUTF8 = new char[lengthUTF8];

	//UnicodeからUTF8へ変換
	WideCharToMultiByte(CP_UTF8, 0, bufUnicode, lenghtUnicode + 1, bufUTF8, lengthUTF8, NULL, NULL);

	std::string strUTF8(bufUTF8);

	delete bufUnicode;
	delete bufUTF8;

	return strUTF8;
}

std::string Network::SHA256(string str)
{
	CryptoPP::SHA256 hash;
	string digest;

	CryptoPP::StringSource s(str, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest))));

	return digest;
}

void Network::setid(int id_)
{
	id = id_;
}
void Network::setname(string name_)
{
	name = name_;
}
int Network::getid()
{
	return id;
}
string Network::getname()
{
	return name;
}