#include "stdafx.h"
#include "MessageController.h"
#include "StringUtil.h"

#pragma comment(lib, "IPHLPAPI.lib")  
#pragma comment(lib, "Psapi.lib")  

#define ZMQ_STATIC
#include <zmq.h>
#ifdef _DEBUG
#pragma comment(lib, "libzmq-static-v140-mt-sgd-4_2_1.lib")
#else
#pragma comment(lib, "libzmq-static-v140-mt-s-4_2_1.lib")
#endif

MessageController::MessageController()
	: zmq_context_(NULL),
	zmq_socket_(NULL),
	connected_(false)
{
}


MessageController::~MessageController()
{
	Finalize();
}

void MessageController::Initialize(std::string const & ip, std::uint32_t port)
{
	if (zmq_context_ == NULL)
		zmq_context_ = zmq_ctx_new();

	ip_ = ip;
	port_ = port;
}

void MessageController::SendMessage(std::string const & msg)
{
	if (zmq_socket_ == NULL)
	{
		int timeout = 50;
		std::string tcp_str = StringUtil::format("tcp://%s:%d", ip_.c_str(), port_);

		zmq_socket_ = zmq_socket(zmq_context_, ZMQ_REQ);
		zmq_setsockopt(zmq_socket_, ZMQ_LINGER, &timeout, sizeof(timeout));
		int result = zmq_connect(zmq_socket_, tcp_str.c_str());
		connected_ = (result == 0 ? true : false);

		if (connected_)
		{
			// send
			zmq_send(zmq_socket_, msg.c_str(), msg.length(), 0);

			// recv
			char buffer[512];
			memset(buffer, 0, sizeof(buffer));
			int wait_to_recv_time = 200;
			while (wait_to_recv_time >= 0)
			{
				--wait_to_recv_time;

				if (zmq_recv(zmq_socket_, buffer, 512, ZMQ_NOBLOCK) >= 0)
					break;

				if (zmq_errno() == ETERM)
					break;

				if (zmq_errno() != EAGAIN)
					break;

				//std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}

		zmq_close(zmq_socket_);
		zmq_socket_ = NULL;
	}
}

void MessageController::Finalize()
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
