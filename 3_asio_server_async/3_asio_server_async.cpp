// 3_asio_server_async.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/functional.hpp>
#include <boost/smart_ptr.hpp>

struct service_helloworld
{
	service_helloworld(boost::asio::io_service & iosev)
		: iosev_(iosev),
		acceptor_(iosev, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 9000))
	{
	}

	void start()
	{
		std::cout << "start accept" << std::endl;
		boost::shared_ptr<boost::asio::ip::tcp::socket> psocket(new boost::asio::ip::tcp::socket(iosev_));
		acceptor_.async_accept(*psocket, boost::bind(&service_helloworld::accept_handler, this, psocket, _1));
	}

	void accept_handler(boost::shared_ptr<boost::asio::ip::tcp::socket> & psocket, boost::system::error_code ec)
	{
		if (ec)
			return;
		std::cout << "accept_handler" << std::end;
		start();
		std::cout << psocket->remote_endpoint().address()<< " " << psocket->remote_endpoint().port() << std::endl;
		boost::shared_ptr<std::string> pstr(new std::string("hello world async!"));
		psocket->async_write_some(boost::asio::buffer(*pstr), boost::bind(&service_helloworld::write_handler, this, pstr, _1, _2));
	}

	void write_handler(boost::shared_ptr<std::string> & pstr, boost::system::error_code ec, size_t bytes_transferred)
	{
		if (ec)
		{
			std::cout << "send failed!" << std::endl;
		}
		else
		{
			std::cout << *pstr << "send successed! size:" << bytes_transferred << std::endl;
		}
	}

private:
	boost::asio::io_service & iosev_;
	boost::asio::ip::tcp::acceptor acceptor_;
};

int main()
{
	boost::asio::io_service iosev;
	service_helloworld sev(iosev);

	
	sev.start();
	iosev.run();
	std::cout << "exit" << std::endl;
    return 0;
}

