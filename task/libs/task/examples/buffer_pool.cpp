
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <string>

#include "boost/task.hpp"
#include "buffer.hpp"

namespace tsk = boost::task;

int main( int argc, char *argv[])
{
	try
	{
		tsk::static_pool< tsk::unbounded_twolock_fifo > pool( tsk::poolsize( 1) );

		int n = 5;	
		tsk::unbounded_buffer< std::string > buf_ping, buf_pong;
		tsk::async(
			tsk::make_task(
				& send_ping,
				n,
				buf_ping,
				buf_pong),
			pool);
		tsk::async(
			tsk::make_task(
				& send_pong,
				n,
				buf_ping,
				buf_pong),
			pool);

		return EXIT_SUCCESS;
	}
	catch ( std::exception const& e)
	{ std::cerr << "exception: " << e.what() << std::endl; }
	catch ( ... )
	{ std::cerr << "unhandled" << std::endl; }

	return EXIT_FAILURE;
}
