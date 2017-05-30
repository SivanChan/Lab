// 1_asio.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <boost/asio.hpp>


int main()
{
	std::cout << "server started!" << std::endl;

	using namespace boost::asio;
	io_service iosev;
	ip::tcp::acceptor acceptor(iosev, ip::tcp::endpoint(ip::tcp::v4(), 9000) );

	for (;;)
	{
		ip::tcp::socket socket(iosev);
		acceptor.accept(socket);

		// 显示连接过来的客户端信息
		std::cout << socket.remote_endpoint().address() << std::endl;

		// 向客户端发送信息
		boost::system::error_code ec;
		socket.write_some(buffer("hello world!"), ec);

		if (ec)
		{
			std::cout << boost::system::system_error(ec).what() << std::endl;
			break;
		}
	}

	std::cout << "server closed!" << std::endl;
    return 0;
}

