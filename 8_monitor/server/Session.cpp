
#include "stdafx.h"
#include <Session.h>
#include <MessagePack.h>
#include <log.h>
#include <StringUtil.h>
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
#include <boost/bind.hpp>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <AppFramework.h>
#include "../MainFrm.h"
#include "../zmq/MessageQueue.h"

namespace Forge
{
	Session::Session(boost::asio::ip::tcp::socket socket)
		: socket_(std::move(socket)),
		header_size_(sizeof(uint8_t) + sizeof(uint32_t)),
		timer_(socket_.get_io_service())
	{
		ip_ = socket_.remote_endpoint().address().to_string();
		ip_port_ = StringUtil::format("%s:%d", socket_.remote_endpoint().address().to_string().c_str(),
			socket_.remote_endpoint().port());
		std::string str = StringUtil::format("%s connected!", ip_port_.c_str());
		Log::Instance().LogMessage(str);

		AppFramework::Instance().GetServerTree()->AddItem(this);
	}

	Session::~Session()
	{
		if (socket_.is_open())
		{
			std::string str = StringUtil::format("%s disconnected!", ip_port_.c_str());
			socket_.close();
			Log::Instance().LogMessage(str);

			AppFramework::Instance().GetServerTree()->RemoveItem(this);
		}
	}

	void Session::Start()
	{ 		
  		auto self(shared_from_this());
  		timer_.expires_from_now(boost::posix_time::seconds(0));
  		timer_.async_wait(boost::bind(&Session::DoHeartBeat, self));
	}

	std::string const & Session::GetIpPortDesc() const
	{
		return ip_port_;
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
				std::string xml_buffer = StringUtil::utf8_to_gbk(msg.GetXML().c_str());

				bool read_header = true;
				rapidxml::xml_document<char>   doc;
				doc.parse<0>((char*)xml_buffer.c_str());

				rapidxml::xml_node<char>* root = doc.first_node("Message");
				if (root != NULL)
				{
					rapidxml::xml_node<char>* body = root->first_node("MessageBody");
					rapidxml::xml_attribute<char> * att = NULL;
					if (body != NULL) 
						att = body->first_attribute("Type");
					if (att != NULL)
					{
						if (std::string(att->value()).compare("HeartBeat") == 0)         // HeartBeat
						{
							DoHeartBeat();
							read_header = false;
						}
						else if (std::string(att->value()).compare("TrafficAlert") == 0) // TrafficAlert
						{
							rapidxml::xml_node<char>* event_alerts = body->first_node("EventAlerts");
							if (event_alerts != NULL)
							{
								rapidxml::xml_node<char>* alert_info = event_alerts->first_node("AlertInfo");
								if (alert_info != NULL)
								{
									std::ostringstream oss;
									rapidxml::print<char>(oss,*alert_info,rapidxml::print_no_indenting);
									AppFramework::AlertInfoPtr info = std::make_shared<AppFramework::AlertInfo>();
									info->alert_type = 1;
									info->camera_ip = ip_;
									info->alert_info = oss.str();
									AppFramework::Instance().Alert(info);
								}
							}
						}
						else if (std::string(att->value()).compare("AbnormalObjAlert") == 0)    // AbnormalObjAlert
						{
							rapidxml::xml_node<char>* event_alerts = body->first_node("AbnormalObjAlerts");
							if (event_alerts!=NULL)
							{
								rapidxml::xml_node<char>* alert_info = event_alerts->first_node("AlertInfo");
								if (alert_info != NULL)
								{
									rapidxml::xml_node<char>* camera_ip = alert_info->first_node("CameraIP");
									if (camera_ip != NULL)
									{
										std::ostringstream oss;
										rapidxml::print<char>(oss, *alert_info, rapidxml::print_no_indenting);
										AppFramework::AlertInfoPtr info = std::make_shared<AppFramework::AlertInfo>();
										info->alert_type = 2;
										info->camera_ip = camera_ip->value();
										info->alert_info = oss.str();
										AppFramework::Instance().Alert(info);
									}
								}
							}
						}
						else
						{

						}
					}
				}

				if (read_header)
				{
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