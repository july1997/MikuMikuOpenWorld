/*
# main server β v0.1.0
#
# ソースの無断転写を禁止します
#
# -- ユーザーのサーバーの移動について --
# 各サーバーが独立して起動する感じです
#
# 移動については一度ログアウトしてから新しい
# 鯖に移動する形とします
#
*/
#include <iostream>

#include "SquareServer.h"

int main()
{
	std::cout << "mikumikuopenworld server β v0.1.0" << std::endl;

	Conect conect;

	//別スレッドで実行
	std::thread t([&conect](){ conect.conect(); });

	//暇つぶし
	for (;;){

		char s[32];
		std::cin.getline(s, 32);

		std::string st(s);

		if (st == std::string("h") || st == std::string("help"))
		{
			std::cout << "help : c , connection -- display the number of connections" << "" << std::endl;
			std::cout << "help : b , ban        -- disconnect connection" << "" << std::endl;
			std::cout << "help : n , name       -- display the handle name" << "" << std::endl;
			std::cout << "help : m , messege    -- send messege" << "" << std::endl;
			std::cout << "help : gc, getcomand  -- get the command, of the connection" << "" << std::endl;
			std::cout << "help : gp, getposition-- get the position, of the connection" << "" << std::endl;
		}

		if (st == std::string("c") || st == std::string("connection"))
		{
			std::cout << "connection : " << conect.getConection() << std::endl;
		}

		if (st == std::string("n") || st == std::string("name"))
		{
			std::cout << "handelname : " << "connection << ";

			char sc[256];
			std::cin.getline(sc, 256);

			int n = std::stoi(sc);

			if (n < conect.sizeInfo())
			{
				std::cout << "handelname : " << conect.getHandelName(n) << " conection[" << n << "]" << std::endl;
			}
			else
			{
				std::cout << "system : " << "no connection" << std::endl;
			}
		}

		if (st == std::string("m") || st == std::string("messege"))
		{
			std::cout << "messege : ";

			char sc[256];
			std::cin.getline(sc, 100);

			std::string stc(sc);

			if (stc.size() > 0)
			{
				conect.inputMessege(-1, sc);

				std::cout << "messege : " << stc << std::endl;
			}
			else
			{
				std::cout << "system : " << "please enter the messege" << std::endl;
			}
		}

		if (st == std::string("b") || st == std::string("ban"))
		{
			std::cout << "ban : " << "connection << ";

			char sc[256];
			std::cin.getline(sc, 256);

			int n = std::stoi(sc);

			if (n < conect.sizeInfo())
			{
				std::cout << "ban : " << " conection[" << n << "]" << std::endl;

				conect.ban(n);
			}
			else
			{
				std::cout << "system : " << "no connection" << std::endl;
			}
		}

		if (st == std::string("gc") || st == std::string("getcomand"))
		{
			std::cout << "getcomand : " << "connection << ";

			char sc[256];
			std::cin.getline(sc, 256);

			int n = std::stoi(sc);

			if (n < conect.sizeInfo())
			{
				std::cout << "command : " << conect.getComand(n) << std::endl;
			}
			else
			{
				std::cout << "system : " << "no connection" << std::endl;
			}
		}

		if (st == std::string("gp") || st == std::string("getposition"))
		{
			std::cout << "getposition : " << "connection << ";

			char sc[256];
			std::cin.getline(sc, 256);

			int n = std::stoi(sc);

			if (n < conect.sizeInfo())
			{
				std::cout << "position : " << conect.getPosition(n) << std::endl;
			}
			else
			{
				std::cout << "system : " << "no connection" << std::endl;
			}
		}

		if (st == std::string("r") || st == std::string("reset"))
		{
			conect.reset();
		}

		if (st == std::string("y") || st == std::string("youtube"))
		{
			std::cout << "youtube : ";

			char sc[256];
			std::cin.getline(sc, 100);

			std::string stc(sc);

			if (stc.size() > 0)
			{
				conect.yotube(stc);
			}
			else
			{
				std::cout << "system : " << "please enter the messege" << std::endl;
			}
		}
	}
}