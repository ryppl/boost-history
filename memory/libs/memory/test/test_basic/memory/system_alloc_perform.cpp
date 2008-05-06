//
//  system_alloc_perform.cpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#include <boost/memory.hpp>
#include <boost/detail/performance_counter.hpp>

template <class LogT>
class TestSystemAlloc
{
private:
	void** p;
	enum { Total = 5000 };

public:
	TestSystemAlloc()
	{
		p = new void*[Total];
	}
	~TestSystemAlloc()
	{
		delete[] p;
	}

	template <class LogT2, class AllocT>
	void doTestAlloc(LogT2& log, AllocT& alloc)
	{
		NS_BOOST_DETAIL::accumulator acc;
		for (int i, j = 0; j < 16; ++j)
		{
			NS_BOOST_DETAIL::performance_counter counter;
			{
				for (i = 0; i < Total; ++i)
				{
					p[i] = alloc.allocate(BOOST_MEMORY_BLOCK_SIZE);
				}
				for (i = 0; i < Total; ++i)
				{
					alloc.deallocate(p[i]);
				}
			}
			acc.accumulate(counter.trace(log));
		}
		acc.trace_avg(log);
	}

	void test(LogT& log)
	{
		NS_BOOST_MEMORY::system_pool_dyn sysPool;
		NS_BOOST_MEMORY::stdlib_alloc stdLib;

		NS_BOOST_DETAIL::null_log nullLog;
		doTestAlloc(nullLog, sysPool);

		log.trace("\n===== StdLibAlloc =====\n");
		doTestAlloc(log, stdLib);

		log.trace("\n===== SystemPoolAlloc =====\n");
		doTestAlloc(log, sysPool);
	}
};

void testSystemAlloc()
{
	typedef NS_BOOST_DETAIL::stdout_log LogT;
	LogT log;
	TestSystemAlloc<LogT> test;
	test.test(log);
}
