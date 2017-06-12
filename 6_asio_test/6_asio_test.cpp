// 6_asio_test.cpp : 定义控制台应用程序的入口点。
//

/*
#include "stdafx.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <boost/array.hpp>

void handler(const boost::system::error_code & ec)
{
	std::cout << "5s" << std::endl;
}

void handler1(const boost::system::error_code &ec)
{
	std::cout << "5 s." << std::endl;
}

void handler2(const boost::system::error_code &ec)
{
	std::cout << "10 s." << std::endl;
}

boost::asio::io_service io_service1;
boost::asio::io_service io_service2;

void run1()
{
	io_service1.run();
}

void run2()
{
	io_service2.run();
}

boost::asio::io_service io_service;
boost::asio::ip::tcp::resolver resolver(io_service);
boost::asio::ip::tcp::socket sock(io_service);
boost::array<char, 4096> buffer;

void read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred)
{
	if (!ec)
	{
		std::cout << std::string(buffer.data(), bytes_transferred) << std::endl;
		sock.async_read_some(boost::asio::buffer(buffer), read_handler);
	}
}

void connect_handler(const boost::system::error_code &ec)
{
	if (!ec)
	{
		boost::asio::write(sock, boost::asio::buffer("GET / HTTP 1.1\r\nHost: highscore.de\r\n\r\n"));
		sock.async_read_some(boost::asio::buffer(buffer), read_handler);
	}
}

void resolve_handler(const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator it)
{
	if (!ec)
	{
		sock.async_connect(*it, connect_handler);
	}
}

int main()
{
	/* 异步通信
	boost::asio::io_service io_service;
	boost::asio::deadline_timer timer(io_service, boost::posix_time::seconds(5));
	timer.async_wait(handler);
	io_service.run();
	*/

	/*
	boost::asio::io_service io_service;
	boost::asio::deadline_timer timer1(io_service, boost::posix_time::seconds(5));
	timer1.async_wait(handler1);
	boost::asio::deadline_timer timer2(io_service, boost::posix_time::seconds(10));
	timer2.async_wait(handler2);
	io_service.run();
	*/

	/*
	boost::asio::deadline_timer timer1(io_service1, boost::posix_time::seconds(5));
	timer1.async_wait(handler1);
	boost::asio::deadline_timer timer2(io_service2, boost::posix_time::seconds(5));
	timer2.async_wait(handler2);
	boost::thread thread1(run1);
	boost::thread thread2(run2);
	thread1.join();
	thread2.join();
	*/
/*
	boost::asio::ip::tcp::resolver::query query("www.highscore.de", "80");
	resolver.async_resolve(query, resolve_handler);
	io_service.run();

    return 0;
}
*/
//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "stdafx.h"
#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// Class to manage the memory to be used for handler-based custom allocation.
// It contains a single block of memory which may be returned for allocation
// requests. If the memory is in use when an allocation request is made, the
// allocator delegates allocation to the global heap.
class handler_allocator
{
public:
	handler_allocator()
		: in_use_(false)
	{
	}

	handler_allocator(const handler_allocator&) = delete;
	handler_allocator& operator=(const handler_allocator&) = delete;

	void* allocate(std::size_t size)
	{
		if (!in_use_ && size < sizeof(storage_))
		{
			in_use_ = true;
			return &storage_;
		}
		else
		{
			return ::operator new(size);
		}
	}

	void deallocate(void* pointer)
	{
		if (pointer == &storage_)
		{
			in_use_ = false;
		}
		else
		{
			::operator delete(pointer);
		}
	}

private:
	// Storage space used for handler-based custom memory allocation.
	/*typename */std::aligned_storage<1024>::type storage_;

	// Whether the handler-based custom allocation storage has been used.
	bool in_use_;
};

// Wrapper class template for handler objects to allow handler memory
// allocation to be customised. Calls to operator() are forwarded to the
// encapsulated handler.
template <typename Handler>
class custom_alloc_handler
{
public:
	custom_alloc_handler(handler_allocator& a, Handler h)
		: allocator_(a),
		handler_(h)
	{
	}

	template <typename ...Args>
	void operator()(Args&&... args)
	{
		handler_(std::forward<Args>(args)...);
	}

	friend void* asio_handler_allocate(std::size_t size,
		custom_alloc_handler<Handler>* this_handler)
	{
		return this_handler->allocator_.allocate(size);
	}

	friend void asio_handler_deallocate(void* pointer, std::size_t /*size*/,
		custom_alloc_handler<Handler>* this_handler)
	{
		this_handler->allocator_.deallocate(pointer);
	}

private:
	handler_allocator& allocator_;
	Handler handler_;
};

// Helper function to wrap a handler object to add custom allocation.
template <typename Handler>
inline custom_alloc_handler<Handler> make_custom_alloc_handler(
	handler_allocator& a, Handler h)
{
	return custom_alloc_handler<Handler>(a, h);
}

class session
	: public std::enable_shared_from_this<session>
{
public:
	session(tcp::socket socket)
		: socket_(std::move(socket))
	{
		std::cout << "connected" << socket_.remote_endpoint().address() << " " << socket_.remote_endpoint().port() << std::endl;
	}

	~session()
	{
		std::cout << "disconnected" << socket_.remote_endpoint().address() << " " << socket_.remote_endpoint().port() << std::endl;
	}

	void start()
	{
		do_read();
	}

private:
	void do_read()
	{
		auto self(shared_from_this());
		socket_.async_read_some(boost::asio::buffer(data_),
			make_custom_alloc_handler(allocator_,
				[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				do_write(length);
			}
		}));
	}

	void do_write(std::size_t length)
	{
		auto self(shared_from_this());
		boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
			make_custom_alloc_handler(allocator_,
				[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				do_read();
			}
		}));
	}

	// The socket used to communicate with the client.
	tcp::socket socket_;

	// Buffer used to store data received from the client.
	std::array<char, 1024> data_;

	// The allocator to use for handler-based custom memory allocation.
	handler_allocator allocator_;
};

class server
{
public:
	server(boost::asio::io_service& io_service, short port)
		: acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
		socket_(io_service)
	{
		do_accept();
	}

private:
	void do_accept()
	{
		acceptor_.async_accept(socket_,
			[this](boost::system::error_code ec)
		{
			if (!ec)
			{
				std::make_shared<session>(std::move(socket_))->start();
			}

			do_accept();
		});
	}

	tcp::acceptor acceptor_;
	tcp::socket socket_;
};

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: server <port>\n";
			return 1;
		}

		boost::asio::io_service io_service;
		server s(io_service, std::atoi(argv[1]));
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
