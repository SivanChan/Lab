
#include "stdafx.h"
#include <Server.h>
#include <Log.h>
#include <functional>
#include <Session.h>

using namespace boost::asio;

namespace Forge
{
	Server::Server(boost::asio::io_service & io_server, boost::asio::ip::tcp::endpoint const & endpoint)
		: acceptor_(io_server, endpoint),
		socket_(io_server)
	{

	}

	void Server::Start()
	{
		acceptor_.async_accept(socket_,
			[this](boost::system::error_code ec)
		{
			if (!ec)
			{
				std::make_shared<Session>(std::move(socket_))->Start();
			}

			Start();
		});
	}
}