
#include "stdafx.h"
#include "MessageQueue.h"
#include <StringUtil.h>
#include <Log.h>

#pragma comment(lib, "IPHLPAPI.lib")  
#pragma comment(lib, "Psapi.lib")  

#define ZMQ_STATIC
#include <zmq.h>
#ifdef _DEBUG
	#pragma comment(lib, "libzmq-static-v140-mt-sgd-4_2_1.lib")
#else
	#pragma comment(lib, "libzmq-static-v140-mt-s-4_2_1.lib")
#endif


namespace Forge
{
	MessageQueue::MessageQueue()
		: zmq_context_(NULL),
		zmq_socket_(NULL),
		connected_(false)
	{

	}

	MessageQueue::~MessageQueue()
	{
		Finalize();
	}

	void MessageQueue::Initialize()
	{
		if (zmq_context_ == NULL)
			zmq_context_ = zmq_ctx_new();
	}

	void MessageQueue::Connect(std::string const & ip, uint32_t port)
	{
		if (zmq_socket_ == NULL)
		{
			int timeout = 50;
			std::string tcp_str = StringUtil::format("tcp://%s:%d", ip.c_str(), port);

			zmq_socket_ = zmq_socket(zmq_context_, ZMQ_REQ);
			zmq_setsockopt(zmq_socket_, ZMQ_LINGER, &timeout, sizeof(timeout));
			int result = zmq_connect(zmq_socket_, tcp_str.c_str());
			connected_ = result == 0 ? true : false;

			std::string msg = StringUtil::format("%s to %s:%d", connected_ ? "connected" : "disconnected", ip.c_str(), port);
			Log::Instance().LogMessage(msg);
		}
	}

	void MessageQueue::Finalize()
	{
		if (zmq_socket_ != NULL)
		{
			zmq_close(zmq_socket_);
			zmq_socket_ = NULL;
		}

		if (zmq_context_ != NULL)
		{
			zmq_ctx_shutdown(zmq_context_);
			zmq_ctx_term(zmq_context_);
			zmq_context_ = NULL;
		}
	}
}