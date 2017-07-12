
#ifndef Block_h__
#define Block_h__

#pragma once

#include <mutex>  
#include <condition_variable>

namespace Forge
{
	/**
		使用c++11实现OpenThreads::Block  
		By [4/7/2017 chenym]
	*/
	class Block
	{
	public:
		Block() : release_(false)
		{
		}

		~Block()
		{
			release();
		}

		inline void block()
		{
			std::unique_lock<std::mutex> lock(mutex_);
			if (!release_)
				condition_.wait(lock);
		}

		inline void block(unsigned long timeout)
		{
			std::unique_lock<std::mutex> lock(mutex_);
			if (!release_)
				condition_.wait_for(lock, std::chrono::milliseconds(timeout));
		}

		inline void release()
		{
			std::unique_lock<std::mutex> lock(mutex_);
			if (!release_)
			{
				release_ = true;
				condition_.notify_all();
			}
		}

		inline void reset()
		{
			std::unique_lock<std::mutex> lock(mutex_);
			release_ = false;
		}

		inline void set(bool do_release)
		{
			if (do_release != release_)
			{
				if (do_release)
				{
					release();
				}
				else
				{
					reset();
				}
			}
		}

	protected:
		std::mutex              mutex_;
		std::condition_variable condition_;
		bool                    release_;

	private:
		Block(Block const &)
		{
		}
	};
}

#endif // Block_h__