#pragma once
//
//通信は必ず一方通行
//クライアントからサーバーに要求する!
//
//必ずDxlibより先にboost系のincludeをしてください
#include <boost/asio.hpp>
#include <boost/thread.hpp>


#include <string>
#include <vector>

#include "Player.h"
#include "NetworkPlayer.h"
#include "Fps.h"
#include "Crypto.h"

namespace asio = boost::asio;
namespace ip = asio::ip;

class Network : public Crypto
{
public:
	Network(Player *_player, NetworkPlayer *_networkplayer);
	
	void crypto();

	void setup(std::string &PlayerHandelName);

	void getLogout(int logoutinfo);
	void getLogoutNun();

	//カウンタを更新して通信する
	void update();

	//内部カウンタの値を獲得する
	int getCounter();

	//コマンド（移動方向）を送る
	void sendCommand();

	//座標を送る
	void sendPos();

	//PlayerNunberのコマンドを得る
	void getCommand(int PlayerNunber);

	void getPosition(int layerNunber);

	void getConection();

	void getMyConection();

	void getHandelName(int PlayerNunber);

	void sendModelFileName(std::string FileName);

	void getModelFileName(int PlayerNunber);

	//チャットの入力をする 戻り値 1:入力中 0:完了
	bool chat();
	void inputChat(std::string String);

	void getChatNun();

	void getChat();

	void MessegePush_back(std::string str);

	//接続を終了
	void close();

	VECTOR getMyChara(std::string &name);
	VECTOR getChara(std::string &name, int PlayerNumber);
	int getPlayerNun();
	int getMycone();

	void deletePlayer(int playernun);

	size_t messege_Size();

	std::string getMessege(int Number);

	void sendYoutubeRequest(std::string VideoURL);
	void getYotubeRequest(std::string command);
	std::vector<std::string> YoutubeMessege;
	std::string YoutubeURL;
	time_t PlayStartTime;
	bool canupdateyotube = 0;
	int sendyotubecommand = 0;
	int Extension=-1;//延長するかしないか

	int connection = 0;
	int myconnection;
	std::vector<std::string> HandelName;
	std::vector<std::string> ModelFileName;

	int havelogoutinfo = 0, haveconectinfo = 0;
	bool logoutflag = 0;

	bool candelete = 0;
	bool deleting = 0;

	std::vector<int> MessegeOrigin;

	//識別コード(ASCIIコード)
	//文字列場合は逆から読む
	//使用不能 \n"0001010" (区切り文字), \t"0001001"(区切り文字)
	//左から1で始まるビットは使用不可
	const char* b_sendCommand		="1000000";
	const char* b_sendPos			="0100000";
	const char* b_close				="0100001";
	const char* b_getCommand		="0100010";
	const char* b_getPosition		="0100011";
	const char* b_getConnection		="0100100";
	const char* b_getmyconnection	="0100101";
	const char* b_getHandelName		="0100110";
	const char* b_getChat			="0100111";
	const char* b_sendHandelname	="0101000";
	const char* b_sendChat			="0101001";
	const char* b_getChatNun		="0101010";
	const char* b_sendModelFileName ="0101100";
	const char* b_getModelFileName	="0101011";
	const char* b_getLogout			="0101110";
	const char* b_getLogoutNun		="0101111";
	const char* b_sendyotubeRequest	="0110000";
	const char* b_getyotubeRequest	="0110001";
	const char* b_initcrypto		="0111110";
	const char* b_crypto			="0111111";

private:

	asio::streambuf send_buffer;
	asio::streambuf receive_buffer;

	Player *player;
	NetworkPlayer *networkplayer;

	//非同期で送信する
	void sendData_async(char command, std::string str);
	//非同期で受信する
	void reserveData_async();
	//暗号化して送信
	void sendDataEncrypt_async(char command, std::string str);

	void send_end(const boost::system::error_code& error);
	void receive_end(const boost::system::error_code& error);

	std::vector<int> ordercommandnun;
	std::vector<int> orderposnun;

	VECTOR oldpos;
	btQuaternion oldrot;
	int sendposnull = 0;
	int sendcomnull = 0 ;

	int counter = 0;

	bool checkdelete = 0,checknewconection=0;

	//チャット用のキー入力
	int InputHandle;
	bool chatflag = 0;

	int haveMessege = 0;
	std::vector<std::string> Messege;
	size_t MessegeNunber = 0;

	bool comVECTOR(VECTOR v1, VECTOR v2);
	bool comQuaternion(btQuaternion &v1, btQuaternion &v2);
};