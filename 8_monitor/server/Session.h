/**
	�Ự
*/
#ifndef Session_h__
#define Session_h__

#pragma once

#include <memory>
#include <array>
#include <Allocator.h>
#include <boost/asio.hpp>
#include <boost/timer.hpp>

namespace Forge
{
	class Session : public std::enable_shared_from_this<Session>
	{
	public:
		Session(boost::asio::ip::tcp::socket socket);
		~Session();

		void Start();
		std::string const & GetIpPortDesc() const;

	protected:
		void DoReadHeader();
		void DoReadBody();
		void DoHeartBeat();

	protected:
		boost::asio::ip::tcp::socket socket_;
		std::string ip_port_;
		std::string buffer_;
		uint32_t const header_size_;
		std::string buffer_heartbeat_;
		boost::asio::deadline_timer timer_;
		boost::timer server_timer_;
	};
}

#endif // Session_h__