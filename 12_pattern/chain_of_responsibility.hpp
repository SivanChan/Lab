
#ifndef chain_of_responsibility_h__
#define chain_of_responsibility_h__

#pragma once

#include <iostream>
#include <memory>

namespace Forge
{
	class Request
	{
	public:
		Request(int level) : level_(level)
		{}

		int GetRequestLevel() const 
		{
			return level_;
		}

	protected:
		int level_;
	};

	class Response
	{
	public:
		std::string res;
	};

	class Handler
	{
	public:
		void SetNext(std::shared_ptr<Handler> const & handler)
		{
			handler_ = handler;
		}

		virtual int GetHandlerLevel() const = 0;

		virtual Response JustDoIt(Request const & req) = 0;

		Response HandlerMessage(Request const & req)
		{
			Response res;

			if (this->GetHandlerLevel() == req.GetRequestLevel())
			{
				return this->JustDoIt(req);
			}
			else
			{
				if (handler_)
				{
					return handler_->HandlerMessage(req);
				} 
				else
				{
					// do something
				}
			}
			return res;
		}

	protected:
		std::shared_ptr<Handler> handler_;
	};


}

#endif // chain_of_responsibility_h__