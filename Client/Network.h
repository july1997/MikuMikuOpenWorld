#pragma once
//
//�ʐM�͕K������ʍs
//�N���C�A���g����T�[�o�[�ɗv������!
//
//�K��Dxlib�����boost�n��include�����Ă�������
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

	//�J�E���^���X�V���ĒʐM����
	void update();

	//�����J�E���^�̒l���l������
	int getCounter();

	//�R�}���h�i�ړ������j�𑗂�
	void sendCommand();

	//���W�𑗂�
	void sendPos();

	//PlayerNunber�̃R�}���h�𓾂�
	void getCommand(int PlayerNunber);

	void getPosition(int layerNunber);

	void getConection();

	void getMyConection();

	void getHandelName(int PlayerNunber);

	void sendModelFileName(std::string FileName);

	void getModelFileName(int PlayerNunber);

	//�`���b�g�̓��͂����� �߂�l 1:���͒� 0:����
	bool chat();
	void inputChat(std::string String);

	void getChatNun();

	void getChat();

	void MessegePush_back(std::string str);

	//�ڑ����I��
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
	int Extension=-1;//�������邩���Ȃ���

	int connection = 0;
	int myconnection;
	std::vector<std::string> HandelName;
	std::vector<std::string> ModelFileName;

	int havelogoutinfo = 0, haveconectinfo = 0;
	bool logoutflag = 0;

	bool candelete = 0;
	bool deleting = 0;

	std::vector<int> MessegeOrigin;

	//���ʃR�[�h(ASCII�R�[�h)
	//������ꍇ�͋t����ǂ�
	//�g�p�s�\ \n"0001010" (��؂蕶��), \t"0001001"(��؂蕶��)
	//������1�Ŏn�܂�r�b�g�͎g�p�s��
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

	//�񓯊��ő��M����
	void sendData_async(char command, std::string str);
	//�񓯊��Ŏ�M����
	void reserveData_async();
	//�Í������đ��M
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

	//�`���b�g�p�̃L�[����
	int InputHandle;
	bool chatflag = 0;

	int haveMessege = 0;
	std::vector<std::string> Messege;
	size_t MessegeNunber = 0;

	bool comVECTOR(VECTOR v1, VECTOR v2);
	bool comQuaternion(btQuaternion &v1, btQuaternion &v2);
};