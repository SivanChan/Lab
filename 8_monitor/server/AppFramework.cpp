
#include "stdafx.h"
#include <AppFramework.h>
#include <fstream>
#include <Util.h>
#include <StringUtil.h>
#include "../zmq/MessageQueue.h"
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
#include "../zmq/Block.h"

#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")

namespace Forge
{
	template<> AppFramework* Singleton<AppFramework>::singleton_ = nullptr;

	AppFramework & AppFramework::Instance()
	{
		assert(singleton_);
		return (*singleton_);
	}

	AppFramework::AppFramework()
		: port_(20001),
		server_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_)),
		vlc_ins_(NULL),
		message_id_(0),
		hwnd_(NULL),
		alert_running_(true)
	{
		// log
		log_ = std::make_shared<Log>();
#ifdef _DEBUG
		log_->Add(std::make_shared<DebugOutputter>());
#endif

		std::string path = StringUtil::format("%s\\log.txt", GetExeDirectory().c_str());
		log_->Add(std::make_shared<FileOutputter>(path));

		// vlc
		const char * vlc_args[] =
		{
			"-I",
			"dummy",
			"--ignore-config",
			"--verbose=2",
		};
		vlc_ins_ = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);

		// msg queue
		msg_queue_ = std::make_shared<MessageQueue>();

		// init config
		InitConfig(config_);
		Log::Instance().LogMessage("read config file!");
	}

	AppFramework::~AppFramework()
	{
		if (config_.need_save)
			SaveAppConfig();

		alert_running_ = false;

		if (alert_thread_)
		{
			block_->release();
			alert_thread_->join();
		}

		if (vlc_ins_ != NULL)
		{
			libvlc_release(vlc_ins_);
		}
	}

	void AppFramework::Start()
	{
		// alert thread
		block_ = std::make_shared<Block>();
		alert_thread_ = std::make_shared<std::thread>(std::bind(&AppFramework::AlertFunc, this));
		Log::Instance().LogMessage("alert thread started!");

		// start zmq connect
		msg_queue_->Initialize("localhost",1984);
		Log::Instance().LogMessage("zmq thread started!");

		// start tcp server
		server_.Start();	
		Log::Instance().LogMessage("server started!");
		io_service_.run();
	}

	void AppFramework::Stop()
	{
		io_service_.stop();
	}

	void AppFramework::SetServerTree(std::shared_ptr<ServerTree> const & tree)
	{
		server_tree_ = tree;
	}

	std::shared_ptr<Forge::ServerTree> const & AppFramework::GetServerTree() const
	{
		return server_tree_;
	}

	libvlc_instance_t * AppFramework::GetVLCInstance()
	{
		return vlc_ins_;
	}

	std::shared_ptr<Forge::MessageQueue> const AppFramework::GetMessageQueue() const
	{
		return msg_queue_;
	}

	void AppFramework::SetMessageID(UINT id)
	{
		message_id_ = id;
	}

	UINT AppFramework::GetMessageID() const
	{
		return message_id_;
	}

	void AppFramework::SetWnd(HWND hWnd)
	{
		hwnd_ = hWnd;
	}

	HWND AppFramework::GetWnd() const
	{
		return hwnd_;
	}

	Forge::AppConfig const & AppFramework::GetAppConfig() const
	{
		return config_;
	}

	Forge::AppConfig & AppFramework::GetAppConfig()
	{
		return config_;
	}

	void AppFramework::Alert(AppFramework::AlertInfoPtr const & info)
	{
		std::lock_guard<std::mutex> lock(alert_mutex_);
		alert_queue_.push(info);
		UpdateBlock();
	}

	void AppFramework::AlertFunc()
	{
		while (alert_running_)
		{
			if (alert_queue_.empty())
				block_->block();
			{
				std::lock_guard<std::mutex> lock(alert_mutex_);
				if (!alert_queue_.empty())
				{
					// do something
					AlertInfoPtr const & info = alert_queue_.front();

					// send to main thread
					SendMessage(hwnd_, message_id_, (WPARAM)info->alert_type, (LPARAM)info.get());

					// zmq

					alert_queue_.pop();
				}
			}
			UpdateBlock();
		}
	}

	void AppFramework::InitConfig(AppConfig & config)
	{
		std::string xml_buffer;
		std::string file_path = StringUtil::format( "%s\\config.xml", GetExeDirectory().c_str() );
		std::ifstream infile(file_path, std::ios::binary);

		if (!infile.is_open())
			return;
		infile.seekg(0,std::ios::end);
		size_t size = static_cast<size_t>(infile.tellg());
		infile.seekg(0);
		xml_buffer.resize(size + 1);
		infile.read(&xml_buffer.front(), static_cast<std::streamsize>(size));
		xml_buffer[size] = 0;
		infile.close();

		rapidxml::xml_document<char>   doc;
		doc.parse<0>((char*)xml_buffer.c_str());
		rapidxml::xml_node<char>* root = doc.first_node("config");
		if (root != NULL)
		{
			rapidxml::xml_node<char>* terminal_id = root->first_node("terminal_id");
			if (terminal_id != NULL)
				config.terminal_id = terminal_id->value();

			rapidxml::xml_node<char>* cameras = root->first_node("cameras");
			if (cameras != NULL)
			{
				rapidxml::xml_attribute<char> * att_port = cameras->first_attribute("port");
				if (att_port != NULL)
					config.camera_port = atoi(att_port->value());

				rapidxml::xml_attribute<char> * att_sub_add = cameras->first_attribute("sub_add");
				if (att_sub_add != NULL)
					config.camera_sub_add = att_sub_add->value();

				rapidxml::xml_node<char>* ip = cameras->first_node("ip");
				while (ip != NULL)
				{
					config.ips.push_back(ip->value());
					ip = ip->next_sibling();
				}
			}

			rapidxml::xml_node<char>* video = root->first_node("video");
			if (video != NULL)
			{
				rapidxml::xml_node<char>* width = video->first_node("width");
				if (width != NULL) 
					config.video_width = atoi(width->value());

				rapidxml::xml_node<char>* height = video->first_node("height");
				if (height != NULL)
					config.video_height = atoi(height->value());

				rapidxml::xml_node<char>* x = video->first_node("x");
				rapidxml::xml_node<char>* y = video->first_node("y");
				if (x != NULL && y != NULL)
				{
					config.custom_pos = true;
					config.x = atoi(x->value());
					config.y = atoi(y->value());
				}
			}

			rapidxml::xml_node<char>* zmq = root->first_node("zmq");
			if (zmq != NULL)
			{
				rapidxml::xml_node<char>* ip = zmq->first_node("ip");
				if (ip != NULL)
					config.zmq_ip = ip->value();

				rapidxml::xml_node<char>* port = zmq->first_node("port");
				if (port != NULL)
					config.zmq_port = atoi(port->value());
			}
		}
	}

	void AppFramework::UpdateBlock()
	{
		block_->set(!alert_queue_.empty());
	}

	void AppFramework::SaveAppConfig()
	{
		rapidxml::xml_document<char> doc;
		rapidxml::xml_node<char>* v = doc.allocate_node(rapidxml::node_pi, doc.allocate_string("xml version='1.0' encoding='UTF-8'"));
		doc.append_node(v);

		rapidxml::xml_node<char>* root = doc.allocate_node(rapidxml::node_element, "config");
		doc.append_node(root);
		rapidxml::xml_node<char>* terminal_id = doc.allocate_node(rapidxml::node_element, "terminal_id", config_.terminal_id.c_str());
		root->append_node(terminal_id);

		rapidxml::xml_node<char>* cameras = doc.allocate_node(rapidxml::node_element, "cameras");
		root->append_node(cameras);
		cameras->append_attribute(doc.allocate_attribute("port", doc.allocate_string(StringUtil::format("%d",config_.camera_port).c_str())));
		cameras->append_attribute(doc.allocate_attribute("sub_add",config_.camera_sub_add.c_str()));
		for (auto const & ip : config_.ips)
		{
			rapidxml::xml_node<char>* ipn = doc.allocate_node(rapidxml::node_element, "ip", ip.c_str());
			cameras->append_node(ipn);
		}

		rapidxml::xml_node<char>* video = doc.allocate_node(rapidxml::node_element, "video");
		root->append_node(video);

		rapidxml::xml_node<char>* width = doc.allocate_node(rapidxml::node_element, "width", doc.allocate_string(StringUtil::format("%d",config_.video_width).c_str()));
		video->append_node(width);
		rapidxml::xml_node<char>* height = doc.allocate_node(rapidxml::node_element, "height", doc.allocate_string(StringUtil::format("%d", config_.video_height).c_str()));
		video->append_node(height);
		if (config_.custom_pos)
		{
			rapidxml::xml_node<char>* x = doc.allocate_node(rapidxml::node_element, "x", doc.allocate_string(StringUtil::format("%d", config_.x).c_str()));
			video->append_node(x);
			rapidxml::xml_node<char>* y = doc.allocate_node(rapidxml::node_element, "y", doc.allocate_string(StringUtil::format("%d", config_.y).c_str()));
			video->append_node(y);
		}

		rapidxml::xml_node<char>* zmq = doc.allocate_node(rapidxml::node_element, "zmq");
		root->append_node(zmq);
		rapidxml::xml_node<char>* ip = doc.allocate_node(rapidxml::node_element, "ip", config_.zmq_ip.c_str());
		zmq->append_node(ip);
		rapidxml::xml_node<char>* port = doc.allocate_node(rapidxml::node_element, "port", doc.allocate_string(StringUtil::format("%d",config_.zmq_port).c_str()));
		zmq->append_node(port);

		std::string xml_str;
		rapidxml::print(std::back_inserter(xml_str), doc, 0);

		std::string file_path = StringUtil::format("%s\\config.xml", GetExeDirectory().c_str());
		std::ofstream outfile(file_path, std::ios::binary);
		outfile.write(xml_str.c_str(),xml_str.length());
	}
}