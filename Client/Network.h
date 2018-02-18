#pragma once

#include "DxLib.h"
#include "Common/fps.h"


#pragma comment( lib, "cryptlib.lib" )
#include <aes.h>
#include <dh.h>
#include <modes.h>
#include <osrng.h>
#include <rsa.h>
#include <randpool.h>
#include <hex.h>
#include <osrng.h>
#include <base64.h>
#include <files.h>
#include <pssr.h>
#include <random>
#include <bitset>

#include <string>
using namespace std;
#include <vector>
#include <thread>


class Network
{
	int nethandle;
	int receiveLoop();

	std::thread t;
	std::vector<std::string> message;

	int netudphandle, recivenetudphandle;
	IPDATA UDPIP;

	int UDP_receiveLoop();
	std::thread u;
	std::vector<std::string> command;

	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption enc; //aes
	CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption dec;
	CryptoPP::RSA::PublicKey pubkey; //ras

	std::string common_key_;
	std::string common_key_iv_;

	string name;
	int id;

public:
	Network();
	~Network();

	void setid(int id_);
	void setname(string name_);
	int getid();
	string getname();

	//--TCP--
	int connect(std::string Ip);

	int send(char command, std::string method, std::string str, bool changeUTF8 = 1, bool base64encode = 1, bool useRSA = 0, bool useAES = 0);

	int receive_start();

	size_t getMassegeSize();
	std::string getMassege();

	//--UDP--
	int makeUDPSocket(std::string Ip);

	void sendUDP(char command, std::string method, string Buffer, bool base64encode = 1, bool useAES = 1);

	int UDP_receive_start();

	size_t getCommandSize();
	std::string getCommand();

	//暗号化
	int getRsaPublicKey(std::string publicKeystr);
	std::string encryptByRsa(std::string str);

	int makeAESKey();
	void decryptionByAes(std::string &str);
	void encryptByAes(std::string &str);

	std::string SHA256(string str);

	//署名と同時にバージョン確認
	string version = "Miku Miku Open World Ver0.0.1";
	bool signature(string str);

	//--変換系--
	std::string convertShiftJIS(const std::string srcUTF8);
	std::string convertUTF8(std::string srcShiftJIS);

	//--コマンド(1byte)--
	unsigned const char comment = 1;
};

