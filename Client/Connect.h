#pragma once

#include <boost/asio.hpp>

#include <string>

namespace asio = boost::asio;
namespace ip = asio::ip;

class Connect
{
protected:

	asio::io_service *io_service;
	ip::tcp::socket socket;

public:
	Connect();
	virtual ~Connect(){};

	//IP�A�h���X�Őڑ�����
	bool connectIP(std::string &IPAdress);

	//�z�X�g�l�[������ڑ�����
	bool connectHostName(std::string &HostName);

};

