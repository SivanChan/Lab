
#ifndef AppFramework_h__
#define AppFramework_h__

#pragma once

#include <boost/asio.hpp>
#include <Server.h>
#include <Log.h>
#include <ServerTree.h>
#include <Singleton.h>
#include <vlc/vlc.h>
#include <thread>
#include <mutex>
#include <queue>

namespace Forge
{
	struct AppConfig
	{
		// config
		std::string terminal_id;
		uint32_t    camera_port;
		std::string camera_sub_add;
		std::vector<std::string> ips;
		std::string zmq_ip;
		uint32_t    zmq_port;
	};

	class Block;
	class MessageQueue;
	class AppFramework : public Singleton<AppFramework>
	{
	public:
		struct AlertInfo
		{
			uint8_t     alert_type;
			std::string camera_ip;
			std::string alert_info;

			AlertInfo() : alert_type(0) {}
		};
		typedef std::shared_ptr<AlertInfo> AlertInfoPtr;


		static AppFramework & Instance();
		AppFramework();
		~AppFramework();

		void Start();
		void Stop();

		void SetServerTree(std::shared_ptr<ServerTree> const & tree);
		std::shared_ptr<ServerTree> const & GetServerTree() const;

		libvlc_instance_t * GetVLCInstance();
		std::shared_ptr<MessageQueue> const GetMessageQueue() const;

		void SetMessageID(UINT id);
		UINT GetMessageID() const;

		void SetWnd(HWND hWnd);
		HWND GetWnd() const;

		AppConfig const & GetAppConfig() const;

		void Alert(AppFramework::AlertInfoPtr const & info);

		// thread func
		void AlertFunc();

	protected:
		void InitConfig(AppConfig & config);
		void UpdateBlock();

	protected:
		uint16_t port_;
		boost::asio::io_service io_service_;
		Server server_;
		LogPtr log_;
		std::shared_ptr<ServerTree> server_tree_;
		libvlc_instance_t *     vlc_ins_;
		UINT                    message_id_;
		HWND                    hwnd_;

		// message queue
		std::shared_ptr<MessageQueue> msg_queue_;
		
		// app config
		AppConfig config_;

		// alert thread
		std::mutex                   alert_mutex_;
		std::shared_ptr<Block>       block_;
		std::shared_ptr<std::thread> alert_thread_;
		bool                         alert_running_;
		std::queue<AlertInfoPtr>     alert_queue_;
	};
	typedef std::shared_ptr<AppFramework> AppFrameworkPtr;
}

#endif // AppFramework_h__