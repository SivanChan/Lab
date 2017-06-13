/**
	·þÎñ¶Ë
*/
#ifndef MonitorServer_h__
#define MonitorServer_h__

#pragma once

#include <boost/asio.hpp>

namespace Forge
{
	class Server
	{
	public:
		Server(boost::asio::io_service & io_server,
			boost::asio::ip::tcp::endpoint const & endpoint);

		void Start();

	protected:
		boost::asio::ip::tcp::acceptor acceptor_;
		boost::asio::ip::tcp::socket   socket_;
	};
}

#endif // MonitorServer_h__