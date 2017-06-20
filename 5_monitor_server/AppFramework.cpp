
#include "stdafx.h"
#include <AppFramework.h>

namespace Forge
{
	AppFramework::AppFramework()
		: port_(20001),
		server_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_))
	{
		log_ = std::make_shared<Log>();
#ifdef _DEBUG
		log_->Add(std::make_shared<DebugOutputter>());
#endif
		log_->Add(std::make_shared<CoutOutputter>());
		log_->Add(std::make_shared<FileOutputter>("log.txt"));
	}

	void AppFramework::Start()
	{
		server_.Start();	
		Log::Instance().LogMessage("server started!");
		io_service_.run();
	}

	void AppFramework::HeartBeat()
	{
	}
}