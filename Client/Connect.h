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

	//IPアドレスで接続する
	bool connectIP(std::string &IPAdress);

	//ホストネームから接続する
	bool connectHostName(std::string &HostName);

};

