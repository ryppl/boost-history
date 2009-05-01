
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <string>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "boost/task.hpp"

namespace pt = boost::posix_time;
namespace tsk = boost::task;

inline
void print_fn( std::string const& msg)
{ printf("%s", msg.c_str() ); }

inline
void long_running_fn()
{ boost::this_thread::sleep( pt::milliseconds( 250) ); }

int main( int argc, char *argv[])
{
	try
	{
		tsk::pool<
			tsk::unbounded_channel< tsk::priority< int > >
		> pool( tsk::poolsize( 1) );

		tsk::async(
			pool,
			tsk::make_task(
				long_running_fn),
			0);
		tsk::async(
			pool,
			tsk::make_task(
				print_fn,
				"This"),
			0);
		tsk::async(
			pool,
			tsk::make_task(
				print_fn,
				"a text.\n"),
			2);
		tsk::async(
			pool,
			tsk::make_task(
				print_fn,
				" is "),
			1);

		return EXIT_SUCCESS;
	}
	catch ( std::exception const& e)
	{ std::cerr << "exception: " << e.what() << std::endl; }
	catch ( ... )
	{ std::cerr << "unhandled" << std::endl; }

	return EXIT_FAILURE;
}
