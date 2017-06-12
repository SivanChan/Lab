
#ifndef AppFramework_h__
#define AppFramework_h__

#pragma once

#include <boost/asio.hpp>
#include <Server.h>
#include <Log.h>

namespace Forge
{
	class AppFramework
	{
	public:
		AppFramework();

		void Start();

	protected:
		uint16_t port_;
		boost::asio::io_service io_service_;
		Server server_;
		LogPtr log_;
	};
	typedef std::shared_ptr<AppFramework> AppFrameworkPtr;
}

#endif // AppFramework_h__