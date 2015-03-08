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

	//Delimiter‚Ü‚ÅóM‚·‚é(“¯Šú)
	std::string receiveEndOfStringsSynchro();

	//•¶š—ñ‚ÉDelimiter‚ğ•t‚¯‚Ä‘—‚é(“¯Šú)
	void sendEndOfStringsSynchro(std::string str);

	//Rsa‚ÌŒöŠJŒ®‚ğ‘—‚é
	void sendRsaPublicKey();

	//Rsa‚ÌˆÃ†‚ğ‰ğ“Ç‚·‚é
	std::string decryptionByRsa(std::string str);

	//AES‚ÌˆÃ†‚ğ‰ğ“Ç‚·‚é
	std::string decryptionByAes(std::string str);

	//AES‚Ì‹¤’ÊŒ®‚ğó‚¯æ‚é
	void receiveAESKey();

	//Rsa‚ÅˆÃ†‰»‚·‚é
	std::string encryptByRsa(std::string strShortString);

	//AES‚ÅˆÃ†‰»‚·‚é
	std::string encryptByAes(std::string plainText);
};

