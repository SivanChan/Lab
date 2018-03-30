
#pragma once

#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>

void test_pool()
{
	{
		boost::pool<> p1(sizeof(int)); // 只能用于内置类型的内存池

		int *p = static_cast<int*>(p1.malloc());
		assert(p1.is_from(p));

		p1.free(p);
	}

	{
		class demo_class
		{
		public:
			demo_class(int i) : i_(i)
			{}

			demo_class() : i_(-2)
			{}

			int i_;
		};

		boost::object_pool<demo_class> p1;

		auto p = p1.malloc();
		assert(p1.is_from(p)); // p中数据未经初始化

		p = p1.construct(2);
	}

	{

	}
}