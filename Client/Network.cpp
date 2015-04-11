#include "Network.h"

#include <bitset>

#include "DxLib.h"

//Network.h���Q�Ƃ��Ă�������

Network::Network(Player *_player, NetworkPlayer *_networkplayer, ObjectManager *_objectmanager)
{
	player = _player;
	networkplayer = _networkplayer;
	objectmanager = _objectmanager;
}

void Network::crypto()
{
	sendRsaPublicKey();

	receiveAESKey();
}

void Network::login()
{
	sendData_async('v', Ver);

	crypto();
}

void Network::setup(std::string &PlayerHandelName)
{
	std::string name(PlayerHandelName);

	if (name.size() == 0)name = std::string("������");

	//���M
	sendData_async((char)std::bitset<7>(b_sendHandelname).to_ulong(), name);

	//���f���t�@�C���̖��O�𑗐M
	sendModelFileName(player->getModelFileName());

	//�S�̂̐ڑ������擾
	getConection();

	//�����̐ڑ��ԍ����擾
	getMyConection();

	//���v���[���[�̏����擾
	for (int i = 0; i < connection; i++)
	{
		if (i != myconnection)
		{
			getHandelName(i);

			getModelFileName(i);
		}
		else
		{
			//�����̖��O
			HandelName.push_back(name);

			//�����̓ǂݍ��񂾃��f���@�C����
			ModelFileName.push_back(player->getModelFileName());
		}
	}

	for (int i = 0; i < connection; i++)
	{
		if (i != myconnection)
		{
			networkplayer->addPlayer(ModelFileName[i].c_str());
		}
		else
		{
			networkplayer->myConnect();
		}
	}

	//���O�A�E�g�̔ԍ��擾
	getLogoutNun();

	//�`���b�g�̔ԍ����擾
	getChatNun();

	size_t i = 0;
	while(!finishobject)
	{
		downloadObject(i);
		downloadObjectPosition(i);
		downloadObjectCommand(i);
		i++;
	}
}

void Network::update()
{
	FpsManager Fps;
	//60fps�ł�
	while (socket.is_open())
	{
		counter++;

		if (counter == 300)
		{
			counter = 0;

			checknewconection = 0;

			checkdelete = 0;

			counter = 0;
		}
		else
		{
			//���W�̎擾�Ƒ��M
			if (counter % 60 == 0)
			{
				sendPos();

				//���W�擾
				for (int i = 0; i < connection; i++)
				{
					if (i != myconnection)
					{
						getPosition(i);
					}
				}

				if (player->vehicle != 0)
				{
					uploadObjectPositon(player->vehicle-1);
				}

				for (int i = 0; i < objectmanager->carSize(); i++)
				{
					if (i != player->vehicle - 1)
					{
						downloadObjectPosition(i);
					}
				}
			}

			//���O�A�E�g���擾
			if (candelete && counter > 200 && !checkdelete)
			{
				deleting = 1;

				logoutflag = 0;

				getLogout(havelogoutinfo);

				if (logoutflag)
				{
					getConection();
				}

				checkdelete = 1;

				deleting = 0;
			}

			//�`���b�g�̍X�V
			if (counter % 90 == 0)
			{
				getChat();

				for (int i = 0; i < connection; i++)
				{
					if (i != myconnection)
					{
						lookOwnership(i);
					}
				}
			}

			//�V�����ڑ��̊m�F
			if (candelete == 1&& counter > 100 && !checknewconection)
			{
				int c = connection;

				getConection();

				//�V�����ڑ�
				if (connection > c)
				{
					deleting = 1;

					for (int i = c; i < connection; i++)
					{
						if (i != myconnection)
						{
							getHandelName(i);

							getModelFileName(i);

							networkplayer->addPlayer(ModelFileName[i].c_str());

							std::string m("���������E�E�E");
							MessegePush_back(HandelName[i] + m);
						}
					}

					deleting = 0;
				}

				checknewconection = 1;
			}

			//�R�}���h�擾
			if (counter % 30 == 0 )
			{
				for (int i = 0; i < connection ; i++)
				{
					if (i != myconnection)
					{
						getCommand(i);
					}
				}

				if (player->vehicle != 0)
				{
					uploadObjectCommand(player->vehicle-1);
				}

				for (int i = 0; i < objectmanager->carSize(); i++)
				{
					if (i != player->vehicle-1)
					{
						downloadObjectCommand(i);
					}
				}
			}

			//�R�}���h���M
			if (player->movestart && player->movecounter == 0)sendcomnull++;
			else sendcomnull = 0;
			if (sendcomnull < 3 )
			{
				sendCommand();
			}

			//youtube�̓��惊�N�G�X�g���m�F
			if (counter == 280 && canupdateyotube==0)
			{
				getYotubeRequest("g");

				if (YoutubeURL.size() != 0)
				{
					getYotubeRequest("t");

					canupdateyotube = 1;

					Extension = -1;
				}
			}
			if (sendyotubecommand != 0)
			{
				switch (sendyotubecommand)
				{
					//�A���P���M
				case 1:
					getYotubeRequest("a");
					sendyotubecommand = 0;
					break;
					//�A���P���ʂ��擾
				case 2:
					getYotubeRequest("b");
					sendyotubecommand = 0;
					break;
					//�Đ��I����m�点��
				case 3:
					getYotubeRequest("f");
					sendyotubecommand = 0;
					canupdateyotube = 0;
					break;
				}
			}
		}

		Fps.controlWaitFps();
	}
}

void Network::sendData_async(char command, std::string str)
{
	io_service->reset();//reset��Y��Ă����s����Ȃ�

	std::string sendstr(command + str);

	std::ostream os(&send_buffer);
	os << sendstr << Delimiter;

	asio::async_write(socket, send_buffer.data(), boost::bind(&Network::send_end, this, _1));

	io_service->run();//run�Y���Ǝ��s���Ȃ�
}

void Network::sendDataEncrypt_async(char command, std::string str)
{
	io_service->reset();

	std::string sendstr(command + str);

	std::string encstr = encryptByAes(sendstr);

	while (encstr.find("EOS") != std::string::npos)
	{
		encstr = (sendstr);
	}

	std::ostream os(&send_buffer);
	os << (char)std::bitset<7>(b_crypto).to_ulong() << encstr << Delimiter;

	asio::async_write(socket, send_buffer.data(), boost::bind(&Network::send_end, this, _1));

	io_service->run();
}

void Network::reserveData_async()
{
	io_service->reset();

	asio::async_read_until(socket, receive_buffer, Delimiter, boost::bind(&Network::receive_end, this, _1));

	io_service->run();
}

void Network::send_end(const boost::system::error_code& error)
{
	if (error) {
		if (socket.is_open())
		{
			//�����ؒf���������ꍇ
			MessegePush_back(std::string("�T�[�o�[����ؒf����܂���"));

			socket.close();
		}
	}
	else
	{
		//printfDx("send : ");
		//printfDx(asio::buffer_cast<const char*>(send_buffer.data()));
		//printfDx("\n");

		//���M�o�b�t�@�̃T�C�Y�������
		send_buffer.consume(send_buffer.size());
	}
}

void Network::receive_end(const boost::system::error_code& error)
{
	if (error) {
		if (socket.is_open())
		{
			//�����ؒf���������ꍇ
			MessegePush_back(std::string("�T�[�o�[����ؒf����܂���"));

			socket.close();
		}
	}
	else
	{
		while (receive_buffer.size() != 0)
		{
			std::string buffer = std::string(boost::asio::buffer_cast<const char*>(receive_buffer.data()), receive_buffer.size());
			auto length = buffer.find(Delimiter);

			if (length != std::string::npos)
			{
				receive_buffer.consume(length + strlen(Delimiter));

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

				//printfDx("receive : ");
				//printfDx(bit.to_string().c_str());
				//printfDx(str.c_str());
				//printfDx("\n");

				//�R�}���h
				if (bit == std::bitset<7>(b_getCommand))
				{
					std::string nstr = str.substr(1, str.length());
					std::bitset<7> bbit(nstr[0]);

					networkplayer->networkSetCommand(ordercommandnun[0], 0, bbit[2]);
					networkplayer->networkSetCommand(ordercommandnun[0], 1, bbit[1]);
					networkplayer->networkSetCommand(ordercommandnun[0], 2, bbit[3]);
					networkplayer->networkSetCommand(ordercommandnun[0], 3, bbit[4]);
					networkplayer->networkSetCommand(ordercommandnun[0], 4, bbit[0]);
					networkplayer->networkSetCommand(ordercommandnun[0], 5, bbit[5]);

					//�擪���폜
					ordercommandnun.erase(ordercommandnun.begin());
				}

				//���W
				if (bit == std::bitset<7>(b_getPosition))
				{
					std::string buf[7];

					if (str.find('\t', 0) != std::string::npos)
					{
						int start = 1;
						int i = 0;
						while (str.find('\t', start) != std::string::npos)
						{
							int f = str.find('\t', start);
							std::string nstr = str.substr(start, f - start);

							buf[i] = nstr;

							start = f + 1;

							i++;
						}

						//�Ō�̒l
						std::string nstr = str.substr(start, str.size() - start);
						buf[6] = nstr;

						btQuaternion q = btQuaternion((float)std::atof(buf[3].c_str()), (float)std::atof(buf[4].c_str()), (float)std::atof(buf[5].c_str()), (float)std::atof(buf[6].c_str()));

						networkplayer->networkSetPosRot(orderposnun[0], VGet((float)std::atof(buf[0].c_str()), (float)std::atof(buf[1].c_str()), (float)std::atof(buf[2].c_str())), q);

						//�擪���폜
						orderposnun.erase(orderposnun.begin());
					}
				}

				if (bit == std::bitset<7>(b_getConnection))
				{
					//�ڑ���
					std::string nstr = str.substr(1, str.length());

					connection = std::stoi(nstr);
				}

				if (bit == std::bitset<7>(b_getmyconnection))
				{
					//�����̐ڑ��ԍ�
					std::string nstr = str.substr(1, str.length());

					myconnection = std::stoi(nstr);

					networkplayer->myconnection = std::stoi(nstr);
				}

				if (bit == std::bitset<7>(b_getHandelName))
				{
					//���v���[���[�̃n���h���l�[��
					HandelName.push_back(str.substr(1, str.length()));
				}

				if (bit == std::bitset<7>(b_getModelFileName))
				{
					//���v���[���[�̃��f���t�@�C���l�[��
					ModelFileName.push_back(str.substr(1, str.length()));
				}

				if (bit == std::bitset<7>(b_getLogout))
				{
					//���O�A�E�g���
					std::string nstr = str.substr(1, str.length());

					if (std::stoi(nstr) < HandelName.size())
					{
						std::string m("���ގ����܂���");
						MessegePush_back(HandelName[std::stoi(nstr)] + m);

						int logout = std::stoi(nstr);

						networkplayer->deletePlayer(logout);

						if (logout < myconnection)
						{
							myconnection--;

							networkplayer->myconnection--;
						}

						deletePlayer(logout);

						logoutflag = 1;
					}
				}

				if (bit == std::bitset<7>(b_getLogoutNun))
				{
					//���O�A�E�g�����擾
					std::string nstr = str.substr(1, str.length());

					havelogoutinfo = std::stoi(nstr);
				}

				//�`���b�g�擾
				if (bit == std::bitset<7>(b_getChat))
				{
					MessegeOrigin.push_back(std::stoi(str.substr(1, str.find('\t'))));

					Messege.push_back(str.substr(str.find('\t')+1, str.length()));

					MessegeNunber++;

					//���b�Z�[�W��50�ȏ�ɂȂ�����擪������
					if (Messege.size() > 50)
					{
						for (size_t i = 50; i < Messege.size(); i++)
						{
							Messege.erase(Messege.begin());

							MessegeOrigin.erase(MessegeOrigin.begin());
						}
					}
				}

				//�`���b�g�ԍ��擾
				if (bit == std::bitset<7>(b_getChatNun))
				{
					std::string nstr = str.substr(1, str.length());

					haveMessege = std::stoi(nstr);
				}

				//yotube
				if (bit == std::bitset<7>(b_getyotubeRequest))
				{
					std::string nstr = str.substr(1, 1);

					if (nstr == "g")
					{
						if (str.substr(2, str.length() - 1) == "1")
						{
							if (!tell)
							{
								YoutubeMessege.push_back("���悪�Đ����ł����̍Đ��܂ł��҂���������");
								tell = 1;
							}
						}
						else if (str.substr(2, str.length() - 1) != "0")
						{
							if (YoutubeURL != str.substr(2, str.length()))
							{
								YoutubeURL = str.substr(2, str.length());

								YoutubeMessege.push_back(YoutubeURL + "�̍Đ����������܂�");

								tell = 0;
							}
						}
					}

					if (nstr == "t")
					{
						PlayStartTime = std::stoi(str.substr(2, str.length()));

						//���{���Ԃɕϊ�
						struct tm gmt;
						errno_t err = localtime_s(&gmt, &PlayStartTime);

						YoutubeMessege.push_back("�Đ��J�n���� �F " + std::to_string(gmt.tm_hour) + " : " + std::to_string(gmt.tm_min) + " . " + std::to_string(gmt.tm_sec));
					}

					if (nstr == "b")
					{
						std::string res = str.substr(2, 1);

						if (std::stoi(res) != 2)
						{
							std::string pas = str.substr(3, str.length());
							int k = std::stoi(pas) - 100;

							if (std::stoi(res) == 1)
							{
								Extension = 1;

								YoutubeMessege.push_back("���� : " + pas + "�^�� : " + std::to_string(-k));

								YoutubeMessege.push_back(" ����̍Đ����������܂�");
							}
							if (std::stoi(res) == 0)
							{
								Extension = 0;

								YoutubeMessege.push_back("���� : " + pas + "�^�� : " + std::to_string(-k));

								YoutubeMessege.push_back(" ����̍Đ����I�����܂�");
							}
						}
						else
						{
							Extension = -1;
						}
					}
				}

				//�I�u�W�F�N�g
				if (bit == std::bitset<7>(b_object))
				{
					std::string buf[7];

					if (str.find('\t', 0) != std::string::npos)
					{
						int start = 1;
						int i = 0;
						while (str.find('\t', start) != std::string::npos)
						{
							int f = str.find('\t', start);
							std::string nstr = str.substr(start, f - start);

							buf[i] = nstr;

							start = f + 1;

							i++;
						}

						//�Ō�̒l
						std::string nstr = str.substr(start, str.size() - start);
						buf[3] = nstr;

						if (str.find("-1") == std::string::npos)
						{
							objectmanager->addObject(std::stoi(buf[0].c_str()), VGet((float)std::atof(buf[1].c_str()), (float)std::atof(buf[2].c_str()), (float)std::atof(buf[3].c_str())));
						}
						else
						{
							finishobject = 1;
						}
					}
					else
					{
						finishobject = 1;
					}
				}

				if (bit == std::bitset<7>(b_objectPosition))
				{
					std::string buf[8];

					if (str.find('\t', 0) != std::string::npos)
					{
						int start = 1;
						int i = 0;
						while (str.find('\t', start) != std::string::npos)
						{
							int f = str.find('\t', start);
							std::string nstr = str.substr(start, f - start);

							buf[i] = nstr;

							start = f + 1;

							i++;
						}

						//�Ō�̒l
						std::string nstr = str.substr(start, str.size() - start);
						buf[7] = nstr;

						if (buf[0] != "-1")
						{
							objectmanager->setPosition(std::stoi(buf[0].c_str()), 
								VGet((float)std::atof(buf[1].c_str()), (float)std::atof(buf[2].c_str()) + 10.f, (float)std::atof(buf[3].c_str())),
								btQuaternion((float)std::atof(buf[4].c_str()), (float)std::atof(buf[5].c_str()), (float)std::atof(buf[6].c_str()), (float)std::atof(buf[7].c_str()))
								);
						}

					}
				}

				if (bit == std::bitset<7>(b_objectCommand))
				{
					std::string buf[8];

					if (str.find('\t', 0) != std::string::npos)
					{
						int start = 1;
						int i = 0;
						while (str.find('\t', start) != std::string::npos)
						{
							int f = str.find('\t', start);
							std::string nstr = str.substr(start, f - start);

							buf[i] = nstr;

							start = f + 1;

							i++;
						}

						//�Ō�̒l
						std::string nstr = str.substr(start, str.size() - start);
						buf[3] = nstr;

						if (buf[0] != "-1")
						{
							objectmanager->drive((float)std::stoi(buf[0].c_str()), (float)std::atof(buf[1].c_str()), (float)std::atof(buf[2].c_str()), (float)std::atof(buf[3].c_str()));
						}
					}
				}

				if (bit == std::bitset<7>(b_objectGetOwnership))
				{
					std::string anstr = str.substr(1, str.find('\t'));

					std::string nnstr = str.substr(str.find('\t') + 1, str.length());

					objectmanager->setOwnership(std::stoi(nnstr), std::stoi(anstr));
				}

				if (bit == std::bitset<7>(b_lookOwnership))
				{
					std::string nstr = str.substr(1, str.length());

					networkplayer->Ownership[orderlookOwnership[0]] = std::stoi(nstr);

					orderlookOwnership.erase(orderlookOwnership.begin());
				}

			}
			else
			{
				receive_buffer.consume(receive_buffer.size());
			}
		}
	}
}

void Network::close()
{
	//�ؒf�R�}���h
	std::bitset<7> bit(b_close);

	//���M
	sendData_async((char)bit.to_ulong(),"");

	io_service->post([this]() { socket.close(); });
}

void Network::getLogout(int logoutinfo)
{
	std::bitset<7> bit(b_getLogout);

	sendData_async(char(bit.to_ulong()), std::to_string(logoutinfo));

	reserveData_async();
}

void Network::getLogoutNun()
{
	std::bitset<7> bit(b_getLogoutNun);

	sendData_async(char(bit.to_ulong()), "");

	reserveData_async();
}

void Network::sendCommand()
{
	std::bitset<7> bit(b_sendCommand);
	bit[0] = player->jump;
	bit[1] = player->right;
	bit[2] = player->left;
	bit[3] = player->backward;
	bit[4] = player->forward;
	bit[5] = player->run;

	sendData_async((char)bit.to_ulong(),"");
}

bool Network::comVECTOR(VECTOR v1, VECTOR v2)
{
	if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)return 1;
	else return 0;
}

bool Network::comQuaternion(btQuaternion &v1, btQuaternion &v2)
{
	if (v1.getX() == v2.getX() && v1.getY() == v2.getY() && v1.getZ() == v2.getZ() && v1.getW() == v2.getW())return 1;
	else return 0;
}

void Network::sendPos()
{
	if (comVECTOR(oldpos, player->getPosBullet()) && comQuaternion(oldrot, player->getRotBullet()))sendposnull++;
	else sendposnull = 0;

	if (sendposnull < 3)
	{
		std::bitset<7> bit(b_sendPos);

		btQuaternion q = player->getRotBullet();
		VECTOR p = player->getPosBullet();

		std::string posx = std::to_string(p.x);
		std::string posy = std::to_string(p.y);
		std::string posz = std::to_string(p.z);

		std::string rotx = std::to_string(q.getX());
		std::string roty = std::to_string(q.getY());
		std::string rotz = std::to_string(q.getZ());
		std::string rotw = std::to_string(q.getW());
		
		std::string os;
		//�����_��P�ʂ܂�
		os << posx.substr(0, posx.find('.') + 2) << '\t';
		os << posy.substr(0, posy.find('.') + 2) << '\t';
		os << posz.substr(0, posz.find('.') + 2) << '\t';
		//�����_��S�ʂ܂�
		os << rotx.substr(0, rotx.find('.') + 5) << '\t';
		os << roty.substr(0, roty.find('.') + 5) << '\t';
		os << rotz.substr(0, rotz.find('.') + 5) << '\t';
		os << rotw.substr(0, rotw.find('.') + 5);

		sendData_async((char)bit.to_ulong(),os);

		oldpos = player->getPosBullet();
		oldrot = player->getRotBullet();
	}
}

void Network::getCommand(int PlayerNunber)
{
	ordercommandnun.push_back(PlayerNunber);

	std::bitset<7> bit(b_getCommand);

	sendData_async((char)bit.to_ulong(), std::to_string(PlayerNunber));

	reserveData_async();
}

void Network::getPosition(int PlayerNunber)
{
	orderposnun.push_back(PlayerNunber);

	std::bitset<7> bit(b_getPosition);

	sendData_async((char)bit.to_ulong(),std::to_string(PlayerNunber));

	reserveData_async();
}

void Network::getConection()
{
	std::bitset<7> bit(b_getConnection);

	sendData_async(char(bit.to_ulong()),"");

	reserveData_async();
}

void Network::getMyConection()
{
	std::bitset<7> bit(b_getmyconnection);

	sendData_async((char)bit.to_ulong(),"");

	reserveData_async();
}

void Network::getHandelName(int PlayerNunber)
{
	std::bitset<7> bit(b_getHandelName);

	sendData_async((char)bit.to_ulong(), std::to_string(PlayerNunber));

	reserveData_async();
}

void Network::sendModelFileName(std::string FileName)
{
	std::bitset<7> bit(b_sendModelFileName);

	sendData_async((char)bit.to_ulong(), FileName);
}

void Network::getModelFileName(int PlayerNunber)
{
	std::bitset<7> bit(b_getModelFileName);

	sendData_async((char)bit.to_ulong(), std::to_string(PlayerNunber));

	reserveData_async();
}

bool Network::chat()
{
	if (chatflag == 0)
	{
		// �L�[���̓n���h�������(�L�����Z���Ȃ��S�p�����L�萔�l���͂���Ȃ�)
		InputHandle = MakeKeyInput(100, FALSE, FALSE, FALSE);

		// �쐬�����L�[���̓n���h�����A�N�e�B�u�ɂ���
		SetActiveKeyInput(InputHandle);

		chatflag = 1;
	}

	// ���̓��[�h��`��
	DrawKeyInputModeString(0, 0);

	// ���͓r���̕������`��
	DrawKeyInputString(0, 0, InputHandle);

	// ���͂��I�����Ă���ꍇ
	if (CheckKeyInput(InputHandle) != 0)
	{
		char String[256];

		// ���͂��ꂽ��������擾
		GetKeyInputString(String, InputHandle);
		
		std::string str(String);

		if (str.size() != 0)
		{
			std::bitset<7> bit(b_sendChat);

			//���M
			sendDataEncrypt_async((char)bit.to_ulong(), std::string(String));
		}

		// �p�ς݂̃C���v�b�g�n���h�����폜����
		DeleteKeyInput(InputHandle);

		chatflag = 0;

		getChat();
	}

	return chatflag;
}

void Network::inputChat(std::string String)
{
	std::string str(String);

	if (str.size() != 0)
	{
		std::bitset<7> bit(b_sendChat);

		//���M
		sendDataEncrypt_async((char)bit.to_ulong(), std::string(String));
	}
}

void Network::MessegePush_back(std::string str)
{
	MessegeOrigin.push_back(-1);

	Messege.push_back(str);

	MessegeNunber++;

	//���b�Z�[�W��50�ȏ�ɂȂ�����擪������
	if (Messege.size() > 50)
	{
		for (size_t i = 50; i < Messege.size(); i++)
		{
			Messege.erase(Messege.begin());
		}
	}
}

void Network::getChat()
{
	std::bitset<7> bit(b_getChat);

	sendData_async((char)bit.to_ulong(), std::to_string(haveMessege));

	reserveData_async();
}

void Network::getChatNun()
{
	std::bitset<7> bit(b_getChatNun);

	sendData_async((char)bit.to_ulong(),"");

	reserveData_async();
}

void Network::sendYoutubeRequest(std::string VideoURL)
{
	YoutubeMessege.push_back(VideoURL + "�����N�G�X�g���܂���");

	std::bitset<7> bit(b_sendyotubeRequest);

	sendData_async((char)bit.to_ulong(), VideoURL);
}

void Network::getYotubeRequest(std::string command)
{
	std::bitset<7> bit(b_getyotubeRequest);

	sendData_async((char)bit.to_ulong(), command);

	if (command != "a" && command != "f")reserveData_async();
}

void Network::downloadObject(size_t objectNunber)
{
	std::bitset<7> bit(b_object);

	sendData_async((char)bit.to_ulong(), std::to_string(objectNunber));

	reserveData_async();
}

void Network::downloadObjectPosition(size_t objectNunber)
{
	std::bitset<7> bit(b_objectPosition);

	sendData_async((char)bit.to_ulong(), std::to_string(objectNunber));

	reserveData_async();
}

void Network::downloadObjectCommand(size_t objectNunber)
{
	std::bitset<7> bit(b_objectCommand);

	sendData_async((char)bit.to_ulong(), std::to_string(objectNunber));

	reserveData_async();
}

void Network::uploadObjectPositon(size_t objectNunber)
{
	std::bitset<7> bit(b_upObjectPosition);

	btQuaternion q = objectmanager->getCarRot(objectNunber);
	VECTOR p = objectmanager->getCarPos(objectNunber);

	std::string posx = std::to_string(p.x);
	std::string posy = std::to_string(p.y);
	std::string posz = std::to_string(p.z);

	std::string rotx = std::to_string(q.getX());
	std::string roty = std::to_string(q.getY());
	std::string rotz = std::to_string(q.getZ());
	std::string rotw = std::to_string(q.getW());

	std::string os;
	//�����_��0�ʂ܂�
	os << std::to_string(objectNunber) << '\t';;
	os << posx.substr(0, posx.find('.') +2) << '\t';
	os << posy.substr(0, posy.find('.') +2) << '\t';
	os << posz.substr(0, posz.find('.') +2) << '\t';
	//�����_��S�ʂ܂�
	os << rotx.substr(0, rotx.find('.') + 5) << '\t';
	os << roty.substr(0, roty.find('.') + 5) << '\t';
	os << rotz.substr(0, rotz.find('.') + 5) << '\t';
	os << rotw.substr(0, rotw.find('.') + 5);

	sendData_async((char)bit.to_ulong(), os);
}

void Network::uploadObjectCommand(size_t objectNunber)
{
	std::bitset<7> bit(b_upObjectCommand);

	VECTOR p = objectmanager->getCarCommand(objectNunber);

	std::string posx = std::to_string(p.x);
	std::string posy = std::to_string(p.y);
	std::string posz = std::to_string(p.z);

	std::string os;
	//�����_��0�ʂ܂�
	os << std::to_string(objectNunber) << '\t';
	os << posx.substr(0, posx.find('.')) << '\t';
	os << posy.substr(0, posy.find('.')) << '\t';
	os << posz.substr(0, posz.find('.') + 5) << '\t';

	sendData_async((char)bit.to_ulong(), os);
}

void Network::getOwnership(size_t objectNunber)
{
	objectmanager->setOwnership(objectNunber, 2);

	std::bitset<7> bit(b_objectGetOwnership);

	sendData_async((char)bit.to_ulong(), std::to_string(objectNunber));

	reserveData_async();
}

void Network::downOwnership(size_t objectNunber)
{
	std::bitset<7> bit(b_objecoutOwnership);

	sendData_async((char)bit.to_ulong(), std::to_string(objectNunber));

	objectmanager->setOwnership(objectNunber,0);
}

void Network::lookOwnership(size_t objectNunber)
{
	orderlookOwnership.push_back(objectNunber);

	std::bitset<7> bit(b_lookOwnership);

	sendData_async((char)bit.to_ulong(), std::to_string(objectNunber));

	reserveData_async();
}

int Network::getCounter()
{
	return counter;
}

size_t Network::messege_Size()
{
	return MessegeNunber;
}

std::string Network::getMessege(int Number)
{
	return Messege[Number];
}

VECTOR Network::getMyChara(std::string &name)
{
	//�X�N���[�����W�ɕϊ�
	VECTOR anhead = ConvWorldPosToScreenPos(VGet(player->getPos().x, player->getPos().y + 22.f, player->getPos().z));

	name = HandelName[myconnection];

	return anhead;
}

int Network::getPlayerNun()
{
	return networkplayer->getModelHandlesSize();
}

int Network::getMycone()
{
	return myconnection;
}

VECTOR Network::getChara(std::string &name, int PlayerNumber)
{
	//�X�N���[�����W�ɕϊ�
	VECTOR anhead = ConvWorldPosToScreenPos(VGet(networkplayer->networkGetPos(PlayerNumber).x, networkplayer->networkGetPos(PlayerNumber).y + 22.f, networkplayer->networkGetPos(PlayerNumber).z));
	
	name = HandelName[PlayerNumber];

	return anhead;
}

void Network::deletePlayer(int playernun_)
{
	std::vector<std::string>::iterator stri = HandelName.begin();
	stri += playernun_;
	HandelName.erase(stri);

	stri = ModelFileName.begin();
	stri += playernun_;
	ModelFileName.erase(stri);
}