// 2_asio_client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <boost/asio.hpp>

int main()
{
	std::cout << "client started!" << std::endl;

	using namespace boost::asio;
	io_service iosev;
	ip::tcp::socket socket(iosev);
	ip::tcp::endpoint ep(ip::address_v4::from_string("127.0.0.1"), 9000);
	boost::system::error_code ec;
	socket.connect(ep,ec);
	if (ec)
	{
		std::cout << boost::system::system_error(ec).what() << std::endl;
		return -1;
	}

	char buf[100];
	size_t len = socket.read_some(buffer(buf), ec);
	std::cout.write(buf, len);

	socket.close();
	std::cout << "client closed!" << std::endl;

	system("pause");

    return 0;
}

