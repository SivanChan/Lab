
#include "stdafx.h"
#include <Server.h>
#include <Log.h>
#include <functional>

using namespace boost::asio;

namespace Forge
{
	Server::Server(boost::asio::io_service & io_server, uint16_t port)
		: io_server_(io_server),
		acceptor_(io_server, ip::tcp::endpoint(ip::tcp::v4(), port))
	{
	}

	void Server::Start()
	{
		std::shared_ptr<ip::tcp::socket> socket = std::make_shared<ip::tcp::socket>(io_server_);
		acceptor_.async_accept(*socket, std::bind(&Server::Accepthandler,this, socket, std::placeholders::_1));
	}

	void Server::Stop()
	{

	}

	void Server::Accepthandler(std::shared_ptr<boost::asio::ip::tcp::socket> & socket, boost::system::error_code ec)
	{

	}
}