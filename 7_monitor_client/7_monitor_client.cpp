// 7_monitor_client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <boost/asio.hpp>
#include <MessagePack.h>
#include <boost/bind.hpp>

using namespace boost::asio;
io_service iosev;
ip::tcp::socket socket_(iosev);
std::string buffer_to_send;

void on_write(boost::system::error_code const & ec, size_t bytes)
{
	Sleep(2000);

	Forge::XMLMessagePack msg;
	std::string wstr = "JustTest";
	msg.SetXML(wstr);
	buffer_to_send.resize(msg.PackSize());
	msg.Encode((char*)buffer_to_send.data(), buffer_to_send.length());

	socket_.async_write_some(buffer(buffer_to_send, buffer_to_send.size()),
		boost::bind(on_write, boost::placeholders::_1, boost::placeholders::_2));
}

void connect_handler(boost::system::error_code const & ec)
{
	Forge::XMLMessagePack msg;
	std::string wstr = "JustTest";
	msg.SetXML(wstr);
	buffer_to_send.resize(msg.PackSize());
	msg.Encode((char*)buffer_to_send.data(), buffer_to_send.length());

	socket_.async_write_some(buffer(buffer_to_send, buffer_to_send.size()),
		boost::bind(on_write, boost::placeholders::_1, boost::placeholders::_2));
}

int main()
{
	std::cout << "client started!" << std::endl;

	ip::tcp::endpoint ep(ip::address_v4::from_string("127.0.0.1"), 20001);
	socket_.async_connect(ep, connect_handler);

	iosev.run();

	return 0;
}
