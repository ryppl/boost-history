
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <vector>

#include <boost/thread.hpp>
#include "boost/task.hpp"

namespace tsk = boost::task;

void sub_task( int i)
{
	fprintf( stderr, "t%d running ...\n", i);
	fprintf( stderr, "t%d finished ...\n", i);
}

void main_task( int n)
{
	std::vector< tsk::handle< void > > vec;
	for ( int i = 0; i < n; ++i)
		vec.push_back(
			tsk::async(
					tsk::make_task(
						& sub_task,
						i),
					tsk::as_sub_task() ) );
	tsk::waitfor_all( vec.begin(), vec.end() );
}

int main( int argc, char *argv[])
{
	try
	{
		tsk::poolsize psize( boost::thread::hardware_concurrency() );
		tsk::static_pool< tsk::unbounded_twolock_fifo > pool( psize);

		fprintf( stderr, "pool-size == %d\n", pool.size() );

		int n = 32;	
		tsk::handle< void > h(
			tsk::async(
				tsk::make_task(
					& main_task,
					n),
				pool) );

		fprintf( stderr, "main thread: waiting for t0 to finish\n");
		h.wait();
		fprintf( stderr, "main thread: t0 finished\n");

		return EXIT_SUCCESS;
	}
	catch ( std::exception const& e)
	{ std::cerr << "exception: " << e.what() << std::endl; }
	catch ( ... )
	{ std::cerr << "unhandled" << std::endl; }

	return EXIT_FAILURE;
}
