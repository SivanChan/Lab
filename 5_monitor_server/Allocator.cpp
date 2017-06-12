
#include "stdafx.h"
#include <Allocator.h>

namespace Forge
{
	HandlerAllocator::HandlerAllocator()
		: in_use_(false)
	{

	}

	void* HandlerAllocator::Allocate(std::size_t size)
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

	void HandlerAllocator::Deallocate(void* pointer)
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
}