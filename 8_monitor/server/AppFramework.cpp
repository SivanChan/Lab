
#include "stdafx.h"
#include <AppFramework.h>

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
		server_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_))
	{
		log_ = std::make_shared<Log>();
#ifdef _DEBUG
		log_->Add(std::make_shared<DebugOutputter>());
#endif
		//log_->Add(std::make_shared<CoutOutputter>());
		log_->Add(std::make_shared<FileOutputter>("log.txt"));
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

}