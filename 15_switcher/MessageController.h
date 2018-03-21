#pragma once

#include <iostream>
#include <stdint.h>

class MessageController
{
public:
	MessageController();
	~MessageController();

	void Initialize(std::string const & ip, std::uint32_t port);

	void SendMessage(std::string const & msg);

protected:
	void Finalize();

private:
	void* zmq_context_;
	void* zmq_socket_;
	bool  connected_;

	std::string    ip_;
	std::uint32_t  port_;
};

