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

	//Delimiterまで受信する(同期)
	std::string receiveEndOfStringsSynchro();

	//文字列にDelimiterを付けて送る(同期)
	void sendEndOfStringsSynchro(std::string str);

	//Rsaの公開鍵を送る
	void sendRsaPublicKey();

	//Rsaの暗号を解読する
	std::string decryptionByRsa(std::string str);

	//AESの暗号を解読する
	std::string decryptionByAes(std::string str);

	//AESの共通鍵を受け取る
	void receiveAESKey();

	//Rsaで暗号化する
	std::string encryptByRsa(std::string strShortString);

	//AESで暗号化する
	std::string encryptByAes(std::string plainText);
};

