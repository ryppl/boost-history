
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <vector>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "boost/task.hpp"

namespace pt = boost::posix_time;
namespace tsk = boost::task;

long serial_fib( long n)
{
	if( n < 2)
		return n;
	else
		return serial_fib( n - 1) + serial_fib( n - 2);
}

class fib_task
{
private:
	long	cutof_;

public:
	fib_task( long cutof)
	: cutof_( cutof)
	{}

	long execute( long n)
	{
		if ( n < cutof_)
		{
			if ( n == 0)
				boost::this_task::delay( pt::seconds( 2) );
			return serial_fib( n);
		}
		else
		{
			tsk::handle< long > h1(
				tsk::async(
					tsk::default_pool(),
					tsk::make_task(
						& fib_task::execute,
						boost::ref( * this),
						n - 1) ) );
			tsk::handle< long > h2(
				tsk::async(
					tsk::default_pool(),
					tsk::make_task(
						& fib_task::execute,
						boost::ref( * this),
						n - 2) ) );
			return h1.get() + h2.get();
		}
	}
};


void parallel_fib( long n)
{
	fib_task a( 5);
	long result = a.execute( n);
	printf("n == %d, fibonnaci == %d\n", n, result);
}

int main( int argc, char *argv[])
{
	try
	{
		for ( int i = 0; i < 10; ++i)
			tsk::async(
				tsk::default_pool(),
				tsk::make_task(
					& parallel_fib,
					i) );

		return EXIT_SUCCESS;
	}
	catch ( std::exception const& e)
	{ std::cerr << "exception: " << e.what() << std::endl; }
	catch ( ... )
	{ std::cerr << "unhandled" << std::endl; }

	return EXIT_FAILURE;
}
