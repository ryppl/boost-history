
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include "boost/task.hpp"
#include "buffer.hpp"

namespace tsk = boost::task;


int main( int argc, char *argv[])
{
	try
	{
		int n = 10;	
		tsk::unbounded_buffer< std::string > buf_ping, buf_pong;	
		tsk::handle< void > h1 = tsk::async(
			tsk::make_task(
				& send_ping,
				n,
				buf_ping,
				buf_pong),
			tsk::new_thread() );
		tsk::handle< void > h2 = tsk::async(
			tsk::make_task(
				& send_pong,
				n,
				buf_ping,
				buf_pong),
			tsk::new_thread() );
		h1.wait();
		h2.wait();

		return EXIT_SUCCESS;
	}
	catch ( std::exception const& e)
	{ std::cerr << "exception: " << e.what() << std::endl; }
	catch ( ... )
	{ std::cerr << "unhandled" << std::endl; }

	return EXIT_FAILURE;
}
