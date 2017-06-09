
#include "stdafx.h"
#include <MonitorServer.h>
#include <Log.h>
#include <functional>

using namespace boost::asio;

namespace Forge
{
	MonitorServer::MonitorServer(boost::asio::io_service & io_server, uint16_t port)
		: io_server_(io_server),
		port_(port),
		acceptor_(io_server, ip::tcp::endpoint(ip::tcp::v4(), port))
	{
	}

	void MonitorServer::Start()
	{
		std::shared_ptr<ip::tcp::socket> socket = std::make_shared<ip::tcp::socket>(io_server_);
		acceptor_.async_accept(*socket, std::bind(&MonitorServer::Accepthandler,this, socket_, _1));
	}

	void MonitorServer::Accepthandler(std::shared_ptr<boost::asio::ip::tcp::socket> & socket, boost::system::error_code ec)
	{

	}
}