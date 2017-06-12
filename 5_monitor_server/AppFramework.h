
#ifndef Forge_AppFramework_hpp__
#define Forge_AppFramework_hpp__

#pragma once

#include <boost/asio.hpp>
#include <Server.h>

namespace Forge
{
	class AppFramework
	{
	public:
		AppFramework();

		void Start();
		void Stop();

	protected:
		boost::asio::io_service io_service_;
		uint16_t port_;
		Server   server_;
	};
}

#endif // Forge_AppFramework_hpp__