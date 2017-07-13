
#ifndef MessageQueue_h__
#define MessageQueue_h__

#pragma once

#include <string>
#include <queue>
#include <thread>
#include <mutex>

namespace Forge
{
	class Block;
	class MessageQueue
	{
	public:
		MessageQueue();
		~MessageQueue();

		void Initialize(std::string const & ip, uint32_t port);
		void AddMessage(std::string const & msg);
		void SendFunc();
		void SendMessage(std::string const & msg);

	protected:
		void Finalize();
		void UpdateBlock();

	private:
		void* zmq_context_;
		void* zmq_socket_;
		bool  connected_;

		std::string ip_;
		uint32_t    port_;

		std::shared_ptr<std::thread> send_thread_;
		std::shared_ptr<Block>       block_;
		std::mutex                   send_msg_mutex_;
		std::queue<std::string>      msg_will_sended_;
		bool                         thread_running_;
	};
}

#endif // MessageQueue_h__