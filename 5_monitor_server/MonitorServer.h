/**
	异步通信服务端
*/
#ifndef MonitorServer_h__
#define MonitorServer_h__

#pragma once

#include <boost/asio.hpp>

namespace Forge
{
	class MonitorServer
	{
	public:
		MonitorServer(boost::asio::io_service & io_server, uint16_t port);

		void Start();


		void Accepthandler(std::shared_ptr<boost::asio::ip::tcp::socket> & socket,
			boost::system::error_code ec);


	protected:
		uint16_t port_;
		boost::asio::io_service &      io_server_;
		boost::asio::ip::tcp::acceptor acceptor_;
	};
}

#endif // MonitorServer_h__