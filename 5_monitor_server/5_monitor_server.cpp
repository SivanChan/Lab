// 5_monitor_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <AppFramework.h>
#include <thread>
#include <boost/bind.hpp>

int main()
{
	Forge::AppFrameworkPtr app = std::make_shared<Forge::AppFramework>();
	//app->Start();
	std::thread thd(boost::bind(&Forge::AppFramework::Start, app));
	thd.join();
    return 0;
}

