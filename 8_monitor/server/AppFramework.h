
#ifndef AppFramework_h__
#define AppFramework_h__

#pragma once

#include <boost/asio.hpp>
#include <Server.h>
#include <Log.h>
#include <ServerTree.h>
#include <Singleton.h>
#include <vlc/vlc.h>

namespace Forge
{
	class AppFramework : public Singleton<AppFramework>
	{
	public:
		static AppFramework & Instance();
		AppFramework();
		~AppFramework();

		void Start();
		void Stop();

		void SetServerTree(std::shared_ptr<ServerTree> const & tree);
		std::shared_ptr<ServerTree> const & GetServerTree() const;

		libvlc_instance_t * GetVLCInstance();

		void SetMessageID(UINT id);
		UINT GetMessageID() const;

		void SetWnd(HWND hWnd);
		HWND GetWnd() const;

	protected:
		uint16_t port_;
		boost::asio::io_service io_service_;
		Server server_;
		LogPtr log_;
		std::shared_ptr<ServerTree> server_tree_;
		libvlc_instance_t *     vlc_ins_;
		UINT                    message_id_;
		HWND                    hwnd_;
	};
	typedef std::shared_ptr<AppFramework> AppFrameworkPtr;
}

#endif // AppFramework_h__