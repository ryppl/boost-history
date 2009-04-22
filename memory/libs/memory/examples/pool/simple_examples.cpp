//
//  simple_examples.cpp
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

// -------------------------------------------------------------------------

#ifndef	countof
#define countof(array)						(sizeof(array)/sizeof(array[0]))
#endif

void testPool()
{
	int i;
	boost::memory::pool alloc(sizeof(int));

	void* p[3000];
	for (i = 0; i < countof(p); ++i)
		p[i] = alloc.allocate();
	for (i = 0; i < countof(p); ++i)
		alloc.deallocate(p[i]);

	void* p1 = alloc.allocate();
	void* p2 = alloc.allocate();
	void* p3 = alloc.allocate();

	alloc.deallocate(p2);
	alloc.deallocate(p1);
	alloc.deallocate(p3);

	//
	// Note: It's ok even if you forget to free allocated memory!
	//
	for (i = 0; i < 1000; ++i)
		alloc.allocate();
}

void testScopedPool()
{
	int i;
	boost::memory::block_pool recycle;
	boost::memory::scoped_pool alloc(recycle, sizeof(int));

	void* p[3000];
	for (i = 0; i < countof(p); ++i)
		p[i] = alloc.allocate();
	for (i = 0; i < countof(p); ++i)
		alloc.deallocate(p[i]);

	void* p1 = alloc.allocate();
	void* p2 = alloc.allocate();
	void* p3 = alloc.allocate();

	alloc.deallocate(p2);
	alloc.deallocate(p1);
	alloc.deallocate(p3);

	//
	// Note: It's ok even if you forget to free allocated memory!
	//
	for (i = 0; i < 1000; ++i)
		alloc.allocate();
}

int main()
{
	NS_BOOST_MEMORY::enableMemoryLeakCheck();

	testPool();
	testScopedPool();
	return 0;
}
