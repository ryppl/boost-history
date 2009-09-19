
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include <boost/bind.hpp>
#include <boost/ref.hpp>

#include "boost/task.hpp"
#include "semaphore.hpp"

namespace tsk = boost::task;


int main( int argc, char *argv[])
{
	try
	{
		tsk::static_pool< tsk::unbounded_twolock_fifo > pool( tsk::poolsize( 1) );

		int n = 10;	
		tsk::semaphore sem_a( 0), sem_b( 0);	
		tsk::async(
			tsk::make_task(
				& start_with_sem_a,
				n,
				boost::ref( sem_a),
				boost::ref( sem_b) ),
			pool);
		tsk::async(
			tsk::make_task( & loop_fn, n),
			pool);
		tsk::async(
			tsk::make_task(
				& start_with_sem_b,
				n,
				boost::ref( sem_a),
				boost::ref( sem_b) ),
			pool);

		return EXIT_SUCCESS;
	}
	catch ( std::exception const& e)
	{ std::cerr << "exception: " << e.what() << std::endl; }
	catch ( ... )
	{ std::cerr << "unhandled" << std::endl; }

	return EXIT_FAILURE;
}
