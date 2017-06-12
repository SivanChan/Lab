/**
	ÄÚ´æ·ÖÅä
*/
#ifndef Allocator_h__
#define Allocator_h__

#pragma once

namespace Forge
{
	class HandlerAllocator
	{
	public:
		HandlerAllocator();

		HandlerAllocator(HandlerAllocator const &) = delete;
		HandlerAllocator & operator = (HandlerAllocator const &) = delete;

		void* Allocate(std::size_t size);

		void Deallocate(void* pointer);

	private:
		typename std::aligned_storage<1024>::type storage_;
		bool in_use_;
	};

	template <typename Handler>
	class CustomAllocHandler
	{
	public:
		CustomAllocHandler(HandlerAllocator & a, Handler h)
			: allocator_(a), handler_(h)
		{

		}

		template <typename ...Args>
		void operator()(Args&&... args)
		{
			handler_(std::forward<Args>(args)...);
		}

		friend void* AsioHandlerAllocate(std::size_t size, CustomAllocHandler<Handler>* this_handler)
		{
			return this_handler->allocator_.Allocate(size);
		}

		friend void AsioHandlerDeallocate(void* pointer, std::size_t, CustomAllocHandler<Handler>* this_handler)
		{
			this_handler->allocator_.Deallocate(pointer);
		}

	private:
		HandlerAllocator& allocator_;
		Handler handler_;
	};

	template <typename Handler>
	inline CustomAllocHandler<Handler>
		MakeCustomAllocHandler(HandlerAllocator & a, Handler  h)
	{
		return CustomAllocHandler<Handler>(a, h);
	}
}

#endif // Allocator_h__