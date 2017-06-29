
#include "stdafx.h"
#include <AppFramework.h>
#include <fstream>
#include <Util.h>
#include <StringUtil.h>

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
		hwnd_(NULL)
	{
		log_ = std::make_shared<Log>();
#ifdef _DEBUG
		log_->Add(std::make_shared<DebugOutputter>());
#endif
		//log_->Add(std::make_shared<CoutOutputter>());

		std::string path = StringUtil::format("%s\\log.txt", GetExeDirectory().c_str());
		log_->Add(std::make_shared<FileOutputter>(path));

		const char * vlc_args[] =
		{
			"-I",
			"dummy",
			"--ignore-config",
			"--verbose=2",
		};
		vlc_ins_ = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
	}

	AppFramework::~AppFramework()
	{
		if (vlc_ins_ != NULL)
		{
			libvlc_release(vlc_ins_);
		}
	}

	void AppFramework::Start()
	{
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
}