
#include "stdafx.h"
#include <Session.h>
#include <MessagePack.h>
#include <log.h>
#include <StringUtil.h>
#include <rapidxml.hpp>
#include <boost/bind.hpp>
#include <algorithm>
#include <iostream>

namespace Forge
{
	Session::Session(boost::asio::ip::tcp::socket socket)
		: socket_(std::move(socket)),
		header_size_(sizeof(uint8_t) + sizeof(uint32_t)),
		timer_(socket_.get_io_service())
	{
		std::string str = StringUtil::format("%s:%d connected!",socket_.remote_endpoint().address().to_string().c_str(), 
			socket_.remote_endpoint().port());
		Log::Instance().LogMessage(str);
	}

	Session::~Session()
	{
		std::string str = StringUtil::format("%s:%d disconnected!", socket_.remote_endpoint().address().to_string().c_str(),
			socket_.remote_endpoint().port());
		socket_.close();
		Log::Instance().LogMessage(str);
	}

	void Session::Start()
	{ 		
  		auto self(shared_from_this());
  		timer_.expires_from_now(boost::posix_time::seconds(0));
  		timer_.async_wait(boost::bind(&Session::DoHeartBeat, self));
	}

	void Session::HeartBeat()
	{

	}

	void Session::DoReadHeader()
	{
		buffer_.clear();
		buffer_.resize(header_size_,'\0');
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
			boost::asio::buffer((char*)buffer_.data() + header_size_, body_size + sizeof(uint8_t)*2),
			[this, self](boost::system::error_code ec, std::size_t )
		{
			if (!ec)
			{
				XMLMessagePack msg;
				msg.Decode((char const *)buffer_.data(), buffer_.size());

				std::string const xml_buffer = msg.GetXML();

				bool read_header = true;
				rapidxml::xml_document<char>   doc;
				doc.parse<0>((char*)xml_buffer.c_str());

				rapidxml::xml_node<char>* root = doc.first_node("Message");
				if (root != NULL)
				{
					rapidxml::xml_node<char>* body = root->first_node("MessageBody");
					if (body)
					{
						rapidxml::xml_attribute<char> * att = body->first_attribute("Type");
						if (att != NULL && std::string(att->value()).compare("HeartBeat") == 0)
						{
							DoHeartBeat();
							read_header = false;
						}
					}
				}

				if (read_header)
				{
					Log::Instance().LogMessage(msg.GetXML());
					DoReadHeader();
				}
			}
		});
	}

	void Session::DoHeartBeat()
	{
		if (buffer_heartbeat_.empty())
		{
			static std::string const xml_str = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<Message>\r\n<MessageHeader>\r\n<Version>PITC001C019.00</Version>\r\n<From>resafety</From>\r\n<System>AID</System>\r\n</MessageHeader>\r\n<MessageBody Type=\"HeartBeat\">\r\n</MessageBody>\r\n</Message>\r\n";
			XMLMessagePack msg;
			msg.SetXML(xml_str);
			buffer_heartbeat_.resize(msg.PackSize());
			msg.Encode((char*)buffer_heartbeat_.data(), buffer_heartbeat_.size());
		}
		if (!buffer_heartbeat_.empty())
		{
			server_timer_.restart();
			auto self(shared_from_this());
			boost::asio::async_write(socket_, boost::asio::buffer((char*)buffer_heartbeat_.data(), buffer_heartbeat_.length()),
				[this, self](boost::system::error_code ec, std::size_t)
			{
				if (!ec)
				{
					DoReadHeader();
				}
			});
		}
	}
}