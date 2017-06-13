/**
	»á»°
*/
#ifndef Session_h__
#define Session_h__

#pragma once

#include <memory>
#include <array>
#include <Allocator.h>
#include <boost/asio.hpp>

namespace Forge
{
	class Session : public std::enable_shared_from_this<Session>
	{
	public:
		Session(boost::asio::ip::tcp::socket socket);
		~Session();

		void Start();

	protected:
		void DoReadHeader();
		void DoReadBody();

	protected:
		boost::asio::ip::tcp::socket socket_;
		std::string buffer_;
		uint32_t const header_size_;
	};
}

#endif // Session_h__