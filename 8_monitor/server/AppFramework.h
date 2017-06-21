
#ifndef AppFramework_h__
#define AppFramework_h__

#pragma once

#include <boost/asio.hpp>
#include <Server.h>
#include <Log.h>
#include <ServerTree.h>
#include <Singleton.h>

namespace Forge
{
	class AppFramework : public Singleton<AppFramework>
	{
	public:
		static AppFramework & Instance();
		AppFramework();

		void Start();
		void Stop();

		void SetServerTree(std::shared_ptr<ServerTree> const & tree);
		std::shared_ptr<ServerTree> const & GetServerTree() const;

	protected:
		uint16_t port_;
		boost::asio::io_service io_service_;
		Server server_;
		LogPtr log_;
		std::shared_ptr<ServerTree> server_tree_;
	};
	typedef std::shared_ptr<AppFramework> AppFrameworkPtr;
}

#endif // AppFramework_h__