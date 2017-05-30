// 1_asio.cpp : �������̨Ӧ�ó������ڵ㡣
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

		// ��ʾ���ӹ����Ŀͻ�����Ϣ
		std::cout << socket.remote_endpoint().address() << std::endl;

		// ��ͻ��˷�����Ϣ
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

