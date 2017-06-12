
#include "stdafx.h"
#include <AppFramework.h>
#include <Log.h>

namespace Forge
{
	AppFramework::AppFramework()
		: port_(8509),
		server_(io_service_, port_)
	{
		LogPtr log = std::make_shared<Log>();
#ifdef _DEBUG
		log->Add(std::make_shared<DebugOutputter>());
#endif
		log->Add(std::make_shared<FileOutputter>("log.txt"));
	}

	void AppFramework::Start()
	{
		server_.Start();
		Log::Instance().LogMessage("server started!");
	}

	void AppFramework::Stop()
	{
		server_.Stop();
		Log::Instance().LogMessage("server stoped!");
	}
}