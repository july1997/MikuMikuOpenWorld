#include "Connect.h"


Connect::Connect()
	: io_service(new asio::io_service())
	, socket(*io_service)
{
}


bool Connect::connectIP(std::string &IPAdress)
{
	try
	{
		socket.connect(ip::tcp::endpoint(ip::address::from_string(IPAdress.c_str()), 31400));

	}
	catch (...) {
		return 1;
	}

	return 0;
}

bool Connect::connectHostName(std::string &HostName)
{
	try
	{
		//名前解決(ホスト名からIPアドレスに変換)する
		ip::tcp::resolver resolver(*io_service);
		ip::tcp::resolver::query query(HostName.c_str(), "31400");

		//ホスト情報を設定する
		ip::tcp::endpoint endpoint(*resolver.resolve(query));

		socket.connect(endpoint);
	}
	catch (...) {
		return 1;
	}
	return 0;
}