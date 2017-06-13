
#include "stdafx.h"
#include <Session.h>
#include <MessagePack.h>
#include <log.h>
#include <StringUtil.h>

namespace Forge
{
	Session::Session(boost::asio::ip::tcp::socket socket)
		: socket_(std::move(socket)),
		header_size_(sizeof(uint8_t) + sizeof(uint32_t))
	{
		std::string str = StringUtil::format("%s:%d connected!",socket_.remote_endpoint().address().to_string().c_str(), 
			socket_.remote_endpoint().port());
		Log::Instance().LogMessage(str);
	}

	Session::~Session()
	{
		std::string str = StringUtil::format("%s:%d disconnected!", socket_.remote_endpoint().address().to_string().c_str(),
			socket_.remote_endpoint().port());
		Log::Instance().LogMessage(str);
	}

	void Session::Start()
	{
		buffer_.resize(header_size_);
		DoReadHeader();
	}

	void Session::DoReadHeader()
	{
		auto self(shared_from_this());
		boost::asio::async_read( socket_,
			boost::asio::buffer((char*)buffer_.data(), header_size_),
				[this, self](boost::system::error_code ec, std::size_t )
		{
			if (!ec)
			{
				DoReadBody();
			}
		});
	}

	void Session::DoReadBody()
	{
		uint32_t body_size = 0;
		memcpy_s((void*)&body_size, sizeof(body_size), buffer_.data() + sizeof(uint8_t), sizeof(body_size));
		buffer_.resize(header_size_ + body_size + sizeof(uint8_t));

		auto self(shared_from_this());
		boost::asio::async_read(socket_,
			boost::asio::buffer((char*)buffer_.data() + header_size_, body_size + sizeof(uint8_t)),
			[this, self](boost::system::error_code ec, std::size_t )
		{
			if (!ec)
			{
				XMLMessagePack msg;
				if ( msg.Decode((char const *)buffer_.data(), buffer_.size()) )
				{ 
					Log::Instance().LogMessage(msg.GetXML());
				}

				DoReadHeader();
			}
		});
	}
}