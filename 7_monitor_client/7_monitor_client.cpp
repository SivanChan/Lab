// 7_monitor_client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <boost/asio.hpp>
#include <MessagePack.h>

int main()
{
	std::cout << "client started!" << std::endl;

	using namespace boost::asio;
	io_service iosev;
	ip::tcp::socket socket(iosev);
	ip::tcp::endpoint ep(ip::address_v4::from_string("127.0.0.1"), 8509);
	boost::system::error_code ec;
	socket.connect(ep, ec);
	if (ec)
	{
		std::cout << boost::system::system_error(ec).what() << std::endl;
		return -1;
	}

	std::string buffer_to_send;

	Forge::XMLMessagePack msg;
	std::string wstr = "JustTest";
	msg.SetXML(wstr);
	buffer_to_send.resize(msg.PackSize());
	msg.Encode((char*)buffer_to_send.data(),buffer_to_send.length());

	size_t len = socket.write_some(buffer(buffer_to_send), ec);
	//std::cout.write(str.c_str(), len);

	// socket.close();
	// std::cout << "client closed!" << std::endl;

	system("pause");
    return 0;
}

