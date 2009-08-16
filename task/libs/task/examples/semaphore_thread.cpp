
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
		int n = 10;	
		tsk::semaphore sem_a( 0), sem_b( 0);	
		tsk::handle< void > h1 = tsk::async(
			tsk::make_task(
				& start_with_sem_a,
				n,
				boost::ref( sem_a),
				boost::ref( sem_b) ),
			tsk::new_thread() );
		tsk::handle< void > h2 = tsk::async(
			tsk::make_task(
				& start_with_sem_b,
				n,
				boost::ref( sem_a),
				boost::ref( sem_b) ),
			tsk::new_thread() );
		h1.get();
		h2.get();

		return EXIT_SUCCESS;
	}
	catch ( std::exception const& e)
	{ std::cerr << "exception: " << e.what() << std::endl; }
	catch ( ... )
	{ std::cerr << "unhandled" << std::endl; }

	return EXIT_FAILURE;
}
