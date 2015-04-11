#include <iostream>
#include <string>
#include <bitset>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <fstream>
#include <vector>
#include <thread>
#include <time.h>
namespace asio = boost::asio;
namespace ip = asio::ip;

#include <aes.h>
#include <dh.h>
#include <modes.h>
#include <osrng.h>
#include <rsa.h>
#include <hex.h>
#include <pssr.h>
#include <osrng.h>
#include <base64.h>
#include <files.h>
#include <random>

class DataBese{

	std::vector<std::string> Command;
	std::vector<std::string> Position;
	std::vector<std::string> HandelName;
	std::vector<std::string> ModelFileName;

	std::vector<std::string> Messege;
	std::vector<int> MessegeOrigin;
	size_t MessagesNunber = 0;

	int connection = 0, sconnection = 0;

	std::vector<std::string> object;
	std::vector<std::string> objectPosition;
	std::vector<std::string> objectCommand;
	std::vector<bool> Ownership;

	//�ڑ���ؒf���ꂽ�R�l�N�V�����̔ԍ�
	std::vector<int> Logout;
	//�ڑ���ؒf���ꂽ�R�l�N�V���������邩�ǂ���
	bool logoutinformation = 0;

public:

	std::vector<bool> hide;
	DataBese()
	{
		//���ƂŊO�u��
		//�^�C�v,�傫��
		object.push_back("1\t0\t0\t\t0");
		//�ʂ��ԍ�,���W
		objectPosition.push_back("0\t60.0\t0.0\t0.0\t0.0000\t0.0000\t0.0000\t1.0000");
		objectCommand.push_back("0\t0\t0\t0.0000");
		Ownership.push_back(0);
	}

	std::vector<std::string> YoutubeURL;
	time_t playtime;
	bool gettime = 0, Questionnairek = 0; int peplecounter = 0;
	int people = 0, Questionnaire = 0, qtimer = 1, finished = 0;
	double  Percentage = -1; bool alcome = 0;
	//�ۑ��p
	std::vector<int> storageLogout;
	std::vector<bool>setup;

	std::string getComnad(int ConnectionNumber)
	{
		return Command[ConnectionNumber];
	}

	std::string getPosition(int ConnectionNumber)
	{
		return Position[ConnectionNumber];
	}

	void inputCommand(int ConnectionNumber, std::string command)
	{
		Command[ConnectionNumber] = command;
	}

	void inputPosition(int ConnectionNumber, std::string position)
	{
		Position[ConnectionNumber] = position;
	}

	void newConection()
	{
		Command.push_back(" ");
		Position.push_back("0.000000\t0.000000\t0.000000\t0.000000\t0.000000\t1.000000");
		HandelName.push_back("������");
		ModelFileName.push_back("model/Tda�~�N���� �L���~�����s/Tda�~�N���� �L���~�����s.mv1");

		hide.push_back(0);
	}

	void addConnect()
	{
		connection++;

		sconnection++;
	}

	int getConnection()
	{
		return connection;
	}

	int getSConnection()
	{
		return sconnection;
	}

	void delteConnection(int your_connection)
	{
		logoutinformation = 1;

		Logout.push_back(your_connection);

		//�ۑ��p
		storageLogout.push_back(your_connection);

		sconnection--;
	}

	void deleteData(int ConnectionNumber)
	{
		std::vector<std::string>::iterator iterator = Command.begin();
		iterator += ConnectionNumber;
		Command.erase(iterator);

		iterator = Position.begin();
		iterator += ConnectionNumber;
		Position.erase(iterator);

		iterator = HandelName.begin();
		iterator += ConnectionNumber;
		HandelName.erase(iterator);

		iterator = ModelFileName.begin();
		iterator += ConnectionNumber;
		ModelFileName.erase(iterator);

		std::vector<bool>::iterator flagi = setup.begin();
		flagi += ConnectionNumber;
		setup.erase(flagi);

		flagi = hide.begin();
		flagi += ConnectionNumber;
		hide.erase(flagi);

		connection--;
	}

	int LogOutInformation()
	{
		return logoutinformation;
	}

	size_t LogOutInformation_Size()
	{
		return Logout.size();
	}

	int getLogOutConnection(int connection_)
	{
		return Logout[connection_];
	}

	void deleteLogout()
	{
		Logout.clear();
	}

	void downLogOutNnotice()
	{
		deleteLogout();

		logoutinformation = 0;
	}

	void inputHandelName(int ConnectionNumber, std::string handelname)
	{
		HandelName[ConnectionNumber] = handelname;
	}

	std::string getHandelName(int ConnectionNumber)
	{
		if (ConnectionNumber == -1)
		{
			return std::string("server");
		}
		else
		{
			return HandelName[ConnectionNumber];
		}
	}

	void inputModelFileName(int ConnectionNumber, std::string modelfilename)
	{
		ModelFileName[ConnectionNumber] = modelfilename;
	}

	std::string getModelFileName(int ConnectionNumber)
	{
		return ModelFileName[ConnectionNumber];
	}

	void inputMessege(int ConnectionNumber, std::string messege)
	{
		Messege.push_back(messege);
		MessegeOrigin.push_back(ConnectionNumber);

		MessagesNunber++;

		//���b�Z�[�W��50�ȏ�ɂȂ�����擪������
		if (Messege.size() == 50)
		{
			Messege.erase(Messege.begin());

			MessegeOrigin.erase(MessegeOrigin.begin());
		}
	}

	std::string getMessege(int ConnectionNumber)
	{
		return Messege[ConnectionNumber];
	}

	size_t getMessege_Size()
	{
		return MessagesNunber;
	}

	int getMessegeOrigin(int ConnectionNumber)
	{
		return MessegeOrigin[ConnectionNumber];
	}

	void resetYotube()
	{
		people = 0;
	}

	std::string getObject(size_t object_nunber)
	{
		return object[object_nunber];
	}

	size_t getObjectSize()
	{
		return object.size();

	}
	std::string getObjectPosition(size_t object_nunber)
	{
		return objectPosition[object_nunber];
	}

	std::string getObjectCommand(size_t object_nunber)
	{
		return objectCommand[object_nunber];
	}

	void inputObjectPosition(size_t object_nunber, std::string _objectPosition)
	{
		objectPosition[object_nunber] = _objectPosition;
	}

	void inputObjectCommand(size_t object_nunber, std::string _objectCommand)
	{
		objectCommand[object_nunber] = _objectCommand;
	}

	bool getOwnership(size_t object_nunber)
	{
		return Ownership[object_nunber];
	}

	void setOwnership(size_t objects, bool flag)
	{
		Ownership[objects] = flag;
	}
};

//���ƂŊO�u��
#define Ver "0010"

class Sever {

	asio::io_service *io_service_;

	ip::tcp::socket socket_;

	asio::streambuf request_buffer_;
	asio::streambuf response_buffer_;

	std::shared_ptr<DataBese> databese;

	int my_conection = 0;

	bool yet = 1;

	bool enteretheyoutube = 0;
	time_t RequestTime;

	CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption enc; //aes
	CryptoPP::RSAES_OAEP_SHA_Encryptor pubkey; //ras

	bool init = 0;

public:

	//�ڑ�
	Sever(std::shared_ptr<DataBese> &databese_)
		: io_service_(new asio::io_service())
		, socket_(*io_service_)
	{
		ip::tcp::acceptor acc(*io_service_, ip::tcp::endpoint(ip::tcp::v4(), 31400));
		acc.accept(socket_);

		databese = databese_;

		if (databese->setup.size() > 0)
		{
			//���͂����I���Ă��Ȃ��l�̐l���𐔂���
			size_t notover = 0;
			for (size_t i = 0; i < databese->setup.size(); i++)if (databese->setup[i] == 0)notover++;

			if (notover != 0)
			{
				my_conection = databese->getConnection() + notover;
			}
			else
			{
				my_conection = databese->getConnection();
			}
		}

		databese->setup.push_back(0);

		request_buffer_.consume(request_buffer_.size());

		std::cout << "connect : " << socket_.remote_endpoint().address().to_string() << " conection[" << my_conection << "]" << std::endl;
	}
	~Sever(){}

	asio::io_service& get_io_service() { return *io_service_; }

	bool login()
	{
		if (!checkVersion())return 0;

		if (!getRsaPublicKey())return 0;

		if (!sendAESKey())return 0;

		receive_start();

		return 0;
	}

	bool checkVersion()
	{
		std::string versiton = receiveEndOfStrings();

		versiton.erase(versiton.begin());

		if (versiton == Ver)
		{
			return 1;
		}
		else
		{
			std::cout << "login error : " << "Version is different " << "Connection[" << my_conection << "]" << std::endl;
			socket_.close();

			return 0;
		}
	}

	bool getRsaPublicKey()
	{
		//rsa���J�����󂯎��
		std::string publicKeystr = receiveEndOfStrings();

		publicKeystr.erase(publicKeystr.begin());

		try
		{
			//���ʌ��̐���
			pubkey = CryptoPP::RSAES_OAEP_SHA_Encryptor(
				CryptoPP::StringSource(publicKeystr, true, new CryptoPP::Base64Decoder));
		}
		catch (CryptoPP::Exception const& e)
		{
			std::cout << "CryptoPP Error : " << e.what() << " Connection[" << my_conection << "]" << std::endl;
			return 0;
		}
		catch (...)
		{
			std::cout << "CryptoPP Unknown Error :  " << "Connection[" << my_conection << "]" << std::endl;
			return 0;
		}

		return 1;
	}

	bool sendAESKey()
	{
		//8 �r�b�g
		byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
		byte iv[CryptoPP::AES::BLOCKSIZE];

		//�񌈒萫�̂��闐��
		std::random_device rnd;
		size_t randomseed = rnd();

		//�����̃V�[�h�𑗂�
		sendEndOfFile(encryptByRsa(std::to_string(randomseed)));

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

		try
		{
			// �Í����I�u�W�F�N�g�̍쐬
			enc.SetKeyWithIV(key, sizeof(key), iv);
		}
		catch (CryptoPP::Exception const& e)
		{
			std::cout << "CryptoPP Error : " << e.what() << " Connection[" << my_conection << "]" << std::endl;
			return 0;
		}
		catch (...)
		{
			std::cout << "CryptoPP Unknown Error :  " << "Connection[" << my_conection << "]" << std::endl;
			return 0;
		}

		return 1;
	}

	std::string encryptByRsa(std::string strShortString)
	{
		CryptoPP::AutoSeededRandomPool rnd;

		std::string cipher;
		// Encryption
		CryptoPP::RSAES_OAEP_SHA_Encryptor e(pubkey);

		CryptoPP::StringSource ss1(strShortString, true,
			new CryptoPP::PK_EncryptorFilter(rnd, e,
			new CryptoPP::StringSink(cipher)
			) // PK_EncryptorFilter
			); // StringSource

		return cipher;
	}

	void sendEndOfFile(std::string cipher)
	{
		response_buffer_.consume(response_buffer_.size());
		std::ostream os(&response_buffer_);
		os << cipher << Delimiter;

		asio::async_write(socket_, response_buffer_.data(), boost::bind(&Sever::send_end, this, _1));
		response_buffer_.consume(response_buffer_.size());
	}

	std::string receiveEndOfStrings()
	{
		boost::system::error_code error;
		asio::read_until(socket_, response_buffer_, Delimiter, error);

		std::string buffer(std::string(boost::asio::buffer_cast<const char*>(response_buffer_.data()), response_buffer_.size()));
		auto length = buffer.find_last_of(Delimiter);

		response_buffer_.consume(response_buffer_.size());

		return buffer.substr(0, length - strlen(Delimiter) + 1);
	}

	//�Í���
	std::string encryptByAes(std::string plainText)
	{
		// �Í����̂��߂̕ϊ��t�B���^�̍쐬
		std::string encText;
		CryptoPP::StreamTransformationFilter encFilter(enc, new CryptoPP::StringSink(encText));

		// �Í���
		encFilter.Put(reinterpret_cast<const byte*>(plainText.c_str()), plainText.size());
		encFilter.MessageEnd();

		return  encText;
	}

	//������
	std::string decryptionByAes(std::string str)
	{
		// �������̂��߂̕ϊ��t�B���^�̍쐬
		std::string decText;
		CryptoPP::StreamTransformationFilter decFilter(enc, new CryptoPP::StringSink(decText));
		decFilter.Put(reinterpret_cast<const byte*>(str.c_str()), str.size());
		//���܂��Ȃ�
		decFilter.MessageEnd();

		return decText;
	}

	void receive_start()
	{
		//�w�肵���p�^�[���̃f�[�^�܂Ŏ�M����
		asio::async_read_until(socket_, request_buffer_, Delimiter, boost::bind(&Sever::receive_end, this, _1));
	}

	void sendCommand(char command, std::string str)
	{
		std::string sendstr(command + str);

		response_buffer_.consume(response_buffer_.size());

		std::ostream os(&response_buffer_);
		os << sendstr << Delimiter;

		asio::async_write(socket_, response_buffer_.data(), boost::bind(&Sever::send_end, this, _1));
	}

	void sendCommandEncrypt(char command, std::string str)
	{
		std::string sendstr(command + str);

		std::string encstr = encryptByAes(sendstr);

		while (encstr.find("EOS") != std::string::npos)
		{
			encstr = encryptByAes(sendstr);
		}

		response_buffer_.consume(response_buffer_.size());

		std::ostream os(&response_buffer_);
		os << (char)std::bitset<7>(b_crypto).to_ulong() << encstr << Delimiter;

		asio::async_write(socket_, response_buffer_.data(), boost::bind(&Sever::send_end, this, _1));
	}


	void send_end(const boost::system::error_code& error)
	{
		if (error) {
			//�G���[���L����
			std::cout << "error : " << error.message() << " conection[" << my_conection << "]" << std::endl;

			databese->delteConnection(my_conection);

			socket_.close();
		}
		else
		{
			response_buffer_.consume(response_buffer_.size());
		}
	}

	void receive_end(const boost::system::error_code& error)
	{
		if (error) {
			//�����ؒf���������ꍇ
			std::cout << "error : " << error.message() << " conection[" << my_conection << "]" << std::endl;

			databese->delteConnection(my_conection);

			socket_.close();

		}
		else {

			std::string buffer = std::string(boost::asio::buffer_cast<const char*>(request_buffer_.data()), request_buffer_.size());
			auto length = buffer.find(Delimiter);

			if (length != std::string::npos)
			{
				request_buffer_.consume(length + strlen(Delimiter));

				std::string buff = buffer.substr(0, length);

				// ���l����r�b�g�W���𐶐�����
				std::bitset<7> bit(buff[0]);

				std::string str;

				if (bit == std::bitset<7>(b_crypto))
				{
					buff.erase(buff.begin());

					//�Í�������
					str = decryptionByAes(buff);

					//�r�b�g���X�V
					bit = std::bitset<7>(str[0]);
				}
				else
				{
					str = buff;
				}

				//�R�}���h�̎擾
				if (bit[6] == 1)
				{
					databese->inputCommand(my_conection, str);
				}

				//���W�̎擾
				if (bit == std::bitset<7>(b_sendPos))
				{
					std::string nstr = str.substr(1, str.length());

					databese->inputPosition(my_conection, nstr);
				}

				//�R�}���h�̑��M
				if (bit == std::bitset<7>(b_getCommand))
				{
					std::string nstr = str.substr(1, str.length());

					if (nstr != "")
					{
						sendCommand((char)std::bitset<7>(b_getCommand).to_ulong(), databese->getComnad(std::stoi(nstr)));
					}
				}

				//���W�̑��M
				if (bit == std::bitset<7>(b_getPosition))
				{
					std::string nstr = str.substr(1, str.length());

					sendCommand((char)std::bitset<7>(b_getPosition).to_ulong(), databese->getPosition(std::stoi(nstr)));
				}

				//�ڑ��ԍ��̑��M
				if (bit == std::bitset<7>(b_getmyconnection))
				{
					sendCommand((char)std::bitset<7>(b_getmyconnection).to_ulong(), std::to_string(my_conection));
				}

				//���O�̑��M
				if (bit == std::bitset<7>(b_getHandelName))
				{
					std::string nstr = str.substr(1, str.length());

					sendCommand((char)std::bitset<7>(b_getHandelName).to_ulong(), databese->getHandelName(std::stoi(nstr)));
				}

				//�ڑ����̑��M
				if (bit == std::bitset<7>(b_getConnection))
				{
					sendCommand((char)std::bitset<7>(b_getConnection).to_ulong(), std::to_string(databese->getSConnection()));
				}

				//�n���h���l�[���̎擾
				if (bit == std::bitset<7>(b_sendHandelname))
				{
					std::string nstr = str.substr(1, str.length());

					databese->inputHandelName(my_conection, nstr);

					std::cout << "name : " << databese->getHandelName(my_conection) << " conection[" << my_conection << "]" << std::endl;
				}

				//���f���t�@�C���̖��O���擾
				if (bit == std::bitset<7>(b_sendModelFileName))
				{
					std::string nstr = str.substr(1, str.length());

					databese->inputModelFileName(my_conection, nstr);

					if (yet)
					{
						databese->addConnect();

						databese->setup[my_conection] = 1;

						yet = 0;
					}

					std::cout << "modelfile : " << databese->getModelFileName(my_conection) << " conection[" << my_conection << "]" << std::endl;
				}

				//���f���t�@�C���̖��O�𑗐M
				if (bit == std::bitset<7>(b_getModelFileName))
				{
					std::string nstr = str.substr(1, str.length());

					sendCommand((char)std::bitset<7>(b_getModelFileName).to_ulong(), databese->getModelFileName(std::stoi(nstr)));
				}

				//�`���b�g�̎擾
				if (bit == std::bitset<7>(b_sendChat))
				{
					std::string nstr = str.substr(1, str.length());

					databese->inputMessege(my_conection, nstr);

					std::cout << "messege : " << databese->getMessege(databese->getMessege_Size() - 1) << " conection[" << my_conection << "]" << std::endl;
				}

				//���O�A�E�g���̑��M
				if (bit == std::bitset<7>(b_getLogout))
				{
					std::string nstr = str.substr(1, str.length());

					std::bitset<7> bit_(b_getLogout);

					if (databese->storageLogout.size() != std::stoi(nstr))
					{
						for (size_t i = std::stoi(nstr); i < databese->storageLogout.size(); i++)
						{
							sendCommand((char)bit_.to_ulong(), std::to_string(databese->storageLogout[i]));
						}
					}
					//�Ō�ɑ�������񐔂𑗂�
					sendCommand((char)std::bitset<7>(b_getLogoutNun).to_ulong(), std::to_string(databese->storageLogout.size()));
				}

				//���O�A�E�g�̔ԍ���������
				if (bit == std::bitset<7>(b_getLogoutNun))
				{
					sendCommand((char)std::bitset<7>(b_getLogoutNun).to_ulong(), std::to_string(databese->storageLogout.size()));
				}

				//�`���b�g�̑��M
				if (bit == std::bitset<7>(b_getChat))
				{
					std::string nstr = str.substr(1, str.length());

					std::bitset<7> bit_(b_getChat);

					for (size_t i = 0; i < databese->getMessege_Size() - std::stoi(nstr); i++)
					{
						sendCommandEncrypt((char)bit_.to_ulong(), std::to_string(databese->getMessegeOrigin(i + std::stoi(nstr))) + '\t' + databese->getHandelName(databese->getMessegeOrigin(i + std::stoi(nstr))) + " : " + databese->getMessege(i + std::stoi(nstr)));
					}

					//�Ō�Ƀ��b�Z�[�W�̍Ō�̔ԍ��𑗂�
					sendCommand((char)std::bitset<7>(b_getChatNun).to_ulong(), std::to_string(databese->getMessege_Size()));
				}

				//���b�Z�[�W�̍Ō�̔ԍ������𑗂�
				if (bit == std::bitset<7>(b_getChatNun))
				{
					sendCommand((char)std::bitset<7>(b_getChatNun).to_ulong(), std::to_string(databese->getMessege_Size()));
				}

				if (bit == std::bitset<7>(b_sendyotubeRequest))
				{
					time_t timer;
					time(&timer);

					if (RequestTime == 0 || RequestTime + 300 < timer)
					{
						//URL
						databese->YoutubeURL.push_back(str.substr(1, str.length()));

						std::cout << "YoutubeRequest : " << str.substr(1, str.length()) << " conection[" << my_conection << "]" << std::endl;

						RequestTime = timer;
					}
				}

				if (bit == std::bitset<7>(b_getyotubeRequest))
				{
					std::string comma = str.substr(1, str.length());

					//URL�𑗂�
					if (comma == "g")
					{
						time_t timer;
						time(&timer);

						if (databese->finished >= databese->people && databese->finished != 0 && databese->people != 0 || databese->people == 0 && databese->gettime == 1)
						{
							//���̓���Đ��Ɍ���������
							databese->YoutubeURL.erase(databese->YoutubeURL.begin());

							databese->gettime = 0;

							databese->people = 0;
							databese->Questionnaire = 0;
							databese->finished = 0;

							databese->Questionnairek = 0;
							databese->qtimer = 1;
							databese->Percentage = -1;
						}

						//�_�E�����[�h���Ԃ����܂��Ă��đҋ@���Ԉȓ��̎��ƌ��܂��Ă��Ȃ���
						if (databese->gettime == 1 && timer < databese->playtime + (t_waitdownload * databese->qtimer) || databese->gettime == 0)
						{
							if (databese->YoutubeURL.size() != 0)
							{
								sendCommand((char)std::bitset<7>(b_getyotubeRequest).to_ulong(), "g" + databese->YoutubeURL[0]);
							}
							else
							{
								sendCommand((char)std::bitset<7>(b_getyotubeRequest).to_ulong(), "g" + std::to_string(0));
							}
						}
						else
						{
							//�������܍Đ���

							sendCommand((char)std::bitset<7>(b_getyotubeRequest).to_ulong(), "g" + std::to_string(1));
						}
					}

					//�Đ��J�n�����𑗂�
					if (comma == "t")
					{
						time_t timer;
						time(&timer);

						//�Đ����鎞���������߂�
						if (!databese->gettime)
						{
							databese->playtime = timer + t_waitdownload;

							databese->gettime = 1;
						}

						//�_�E�����[�h�ҋ@���Ԉȓ�
						if (timer < databese->playtime + (t_waitdownload * databese->qtimer))
						{
							databese->people++;

							enteretheyoutube = 1;
						}

						sendCommand((char)std::bitset<7>(b_getyotubeRequest).to_ulong(), "t" + std::to_string(databese->playtime));
					}

					//�����ɔ���
					if (comma == "a")
					{
						time_t timer;
						time(&timer);

						//�A���P�[�g�W�v���Ԉȓ�
						if (timer < databese->playtime + (t_limitenquetegettime * databese->qtimer))
						{
							databese->Questionnaire++;
						}
					}

					//�A���P�[�g�̌��ʂ𑗂�
					if (comma == "b")
					{
						time_t timer;
						time(&timer);

						if (databese->qtimer == -1)
						{
							sendCommand((char)std::bitset<7>(b_getyotubeRequest).to_ulong(), "b" + std::to_string(0) + std::to_string((int)(databese->Percentage * 100)));
						}
						else
						{
							//�A���P�[�g�W�v���Ԃ𒴂��Ă���
							if (timer >= databese->playtime + (t_limitenquetegettime * databese->qtimer))
							{
								if (databese->alcome)
								{
									databese->Percentage = -1;
								}

								if (databese->Percentage == -1)
								{
									databese->Percentage = (double)databese->Questionnaire / (double)databese->people;

									if (databese->Percentage > 0.5)
									{
										//�������Ȃ�
										databese->Questionnairek = 0;

										databese->qtimer = -1;
									}
									else
									{
										//����
										databese->Questionnairek = 1;

										databese->qtimer++;
									}

									databese->Questionnaire = 0;
								}

								databese->alcome = 1;

								sendCommand((char)std::bitset<7>(b_getyotubeRequest).to_ulong(), "b" + std::to_string(databese->Questionnairek) + std::to_string((int)(databese->Percentage * 100)));
							}
							else
							{
								if (databese->alcome == 1)
								{
									if (databese->Questionnairek == 0)
									{
										sendCommand((char)std::bitset<7>(b_getyotubeRequest).to_ulong(), "b" + std::to_string(0) + std::to_string((int)(databese->Percentage * 100)));
									}
									else
									{
										if (timer < databese->playtime + (t_limitenquetegettime * databese->qtimer))
										{
											sendCommand((char)std::bitset<7>(b_getyotubeRequest).to_ulong(), "b" + std::to_string(databese->Questionnairek) + std::to_string((int)(databese->Percentage * 100)));
										}
										else
										{
											sendCommand((char)std::bitset<7>(b_getyotubeRequest).to_ulong(), "b" + std::to_string(2));
										}
									}
								}
								else
								{
									//�A���P�[�g�W�v���ԓ�
									sendCommand((char)std::bitset<7>(b_getyotubeRequest).to_ulong(), "b" + std::to_string(2));
								}
							}
						}
					}

					//�Đ��I��
					if (comma == "f")
					{
						if (databese->gettime)databese->finished++;
					}
				}
								
				//�I�u�W�F�N�g
				if (bit == std::bitset<7>(b_object))
				{
					std::string nstr = str.substr(1, str.length());

					if (std::stoi(nstr) < databese->getObjectSize())
					{
						sendCommand((char)std::bitset<7>(b_object).to_ulong(), databese->getObject(std::stoi(nstr)));
					}
					else
					{
						sendCommand((char)std::bitset<7>(b_object).to_ulong(), "-1");
					}
				}

				if (bit == std::bitset<7>(b_objectPosition))
				{
					std::string nstr = str.substr(1, str.length());

					if (std::stoi(nstr) < databese->getObjectSize())
					{
						sendCommand((char)std::bitset<7>(b_objectPosition).to_ulong(), databese->getObjectPosition(std::stoi(nstr)));
					}
					else
					{
						sendCommand((char)std::bitset<7>(b_object).to_ulong(), "-1");
					}
				}

				if (bit == std::bitset<7>(b_objectCommand))
				{
					std::string nstr = str.substr(1, str.length());

					if (std::stoi(nstr) < databese->getObjectSize())
					{
						sendCommand((char)std::bitset<7>(b_objectCommand).to_ulong(), databese->getObjectCommand(std::stoi(nstr)));
					}
					else
					{
						sendCommand((char)std::bitset<7>(b_object).to_ulong(), "-1");
					}
				}

				if (bit == std::bitset<7>(b_upObjectPosition))
				{
					std::string nstr = str.substr(1, str.length());

					auto k = nstr.find('\t');

					if (databese->getOwnership(std::stoi(str.substr(1, k))) == 1)
					{
						if (nstr.find('\t') != std::string::npos)
						{
							std::string nnstr = str.substr(k, str.length());

							databese->inputObjectPosition(std::stoi(str.substr(1, k)), nnstr);
						}
					}
				}

				if (bit == std::bitset<7>(b_upObjectCommand))
				{
					std::string nstr = str.substr(1, str.length());

					auto k = nstr.find('\t');

					if (databese->getOwnership(std::stoi(str.substr(1, k))) == 1)
					{
						if (nstr.find('\t') != std::string::npos)
						{
							std::string nnstr = str.substr(k, str.length());

							databese->inputObjectCommand(std::stoi(str.substr(1, k)), nnstr);
						}
					}
				}

				if (bit == std::bitset<7>(b_objectGetOwnership))
				{
					std::string nstr = str.substr(1, str.length());

					sendCommand((char)std::bitset<7>(b_objectGetOwnership).to_ulong(), nstr + '\t' + std::to_string(databese->getOwnership(std::stoi(nstr))));

					if (databese->getOwnership(std::stoi(nstr)) == 0)
					{
						databese->setOwnership(std::stoi(nstr), 1);

						databese->hide[my_conection] = 1;
					}
				}

				if (bit == std::bitset<7>(b_objecoutOwnership))
				{
					std::string nstr = str.substr(1, str.length());

					databese->setOwnership(std::stoi(nstr), 0);

					databese->hide[my_conection] = 0;
				}

				if (bit == std::bitset<7>(b_lookOwnership))
				{
					std::string nstr = str.substr(1, str.length());

					sendCommand((char)std::bitset<7>(b_lookOwnership).to_ulong(), std::to_string(databese->hide[std::stoi(nstr)]));
				}

				//�ؒf�R�[�h
				if (bit == std::bitset<7>(b_close))
				{
					std::cout << "close : " << "conection[" << my_conection << "]" << std::endl;

					//youtube
					if (enteretheyoutube)databese->people--;

					databese->delteConnection(my_conection);

					socket_.close();
				}
				else
				{
					//��M
					receive_start();
				}
			}
			else
			{
				request_buffer_.consume(request_buffer_.size());
			}
		}
	}

	//�ؒf����
	void ban()
	{
		databese->delteConnection(my_conection);

		socket_.close();
	}

	int getConection()
	{
		return my_conection;
	}

	int getMyConnection()
	{
		return my_conection;
	}

	void updateConnction(int connection)
	{
		std::cout << "update_connect : " << "conection[" << connection << "] " << "<< conection[" << my_conection << "]" << std::endl;

		my_conection = connection;
	}

	//���ʃR�[�h(ASCII�R�[�h)
	//������ꍇ�͋t����ǂ�
	//�g�p�s�\ \n"0001010" (��؂蕶��), \t"0001001"(��؂蕶��)
	//������1�Ŏn�܂�r�b�g�͎g�p�s��
	const char* b_sendCommand = "1000000";
	const char* b_sendPos = "0100000";
	const char* b_close = "0100001";
	const char* b_getCommand = "0100010";
	const char* b_getPosition = "0100011";
	const char* b_getConnection = "0100100";
	const char* b_getmyconnection = "0100101";
	const char* b_getHandelName = "0100110";
	const char* b_getChat = "0100111";
	const char* b_sendHandelname = "0101000";
	const char* b_sendChat = "0101001";
	const char* b_getChatNun = "0101010";
	const char* b_sendModelFileName = "0101100";
	const char* b_getModelFileName = "0101011";
	const char* b_getLogout = "0101110";
	const char* b_getLogoutNun = "0101111";
	const char* b_sendyotubeRequest = "0110000";
	const char* b_getyotubeRequest = "0110001";
	const char* b_checVer = "0110010";
	const char* b_object = "0110011";
	const char* b_objectPosition = "0110100";
	const char* b_objectCommand = "0110101";
	const char* b_upObjectPosition = "0110110";
	const char* b_upObjectCommand = "0110111";
	const char* b_objectGetOwnership = "0111000";
	const char* b_objecoutOwnership = "0111001";
	const char* b_lookOwnership = "0111010";
	const char* b_crypto = "0111111";

	//�_�E�����[�h��҂��ԁi�b�j
	const int t_waitdownload = 120;
	//�A���P�[�g�����W���鎞�ԁi�b�j�N���C�A���g���ƍ��킹�Ă�������
	const int t_limitenquetegettime = 300;

	//�f�~���^
	const char* Delimiter = "EOS";
};

class Conect{

	std::vector<std::thread> process;
	std::vector<std::shared_ptr<Sever>> server;
	std::shared_ptr<DataBese>data = std::make_shared<DataBese>();//���L���邽�߂�

public:

	void conect()
	{
		while (1)
		{
			//�ؒf���ꂽ�R�l�N�V�������������m�F
			if (data->LogOutInformation())
			{
				//��������폜
				for (size_t i = 0; i < data->LogOutInformation_Size(); i++)
				{
					data->deleteData(data->getLogOutConnection(i));

					std::vector<std::shared_ptr<Sever>>::iterator iterator = server.begin();
					iterator += data->getLogOutConnection(i);

					server.erase(iterator);

					//�v���Z�X�̏I����҂�
					process[data->getLogOutConnection(i)].join();

					std::vector<std::thread>::iterator pi = process.begin();
					pi += data->getLogOutConnection(i);

					process.erase(pi);
				}

				//my_conection�̍X�V
				for (size_t j = 0; j < server.size(); j++)
				{
					if (j != server[j]->getMyConnection())server[j]->updateConnction(j);
				}

				//�폜����
				data->downLogOutNnotice();
			}

			//�ڑ��ҋ@�̊J�n
			server.push_back(std::shared_ptr<Sever>(new Sever(data)));

			int connection = data->setup.size() - 1;

			//��M�J�n
			server[connection]->login();

			process.push_back(std::thread(boost::bind(&boost::asio::io_service::run, boost::ref(server[connection]->get_io_service()))));

			//�V���Ȑڑ���`����
			data->newConection();

		}
	}

	int getConection()
	{
		return data->getConnection();
	}

	int sizeInfo()
	{
		return server.size();
	}

	//�ؒf����
	void ban(int ConnectionNumber)
	{
		server[ConnectionNumber]->ban();
	}

	std::string getComand(int ConnectionNumber)
	{
		return data->getComnad(ConnectionNumber);
	}

	std::string getPosition(int ConnectionNumber)
	{
		return data->getPosition(ConnectionNumber);
	}

	std::string getHandelName(int ConnectionNumber)
	{
		return data->getHandelName(ConnectionNumber);
	}

	void inputMessege(int ConnectionNumber, std::string str)
	{
		data->inputMessege(ConnectionNumber, str);
	}

	void reset()
	{
		data->resetYotube();
	}

	void yotube(std::string str)
	{
		data->YoutubeURL.push_back(str);
	}
};