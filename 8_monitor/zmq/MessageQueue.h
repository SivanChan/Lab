
#ifndef MessageQueue_h__
#define MessageQueue_h__

#pragma once

#include <string>

namespace Forge
{
	class MessageQueue
	{
	public:
		MessageQueue();
		~MessageQueue();

		void Initialize();

		void Connect(std::string const & ip, uint32_t port);

	protected:
		void Finalize();

	private:
		void* zmq_context_;
		void* zmq_socket_;
		bool  connected_;
	};
}

#endif // MessageQueue_h__