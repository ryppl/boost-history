
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

typedef tsk::static_pool< tsk::unbounded_channel< tsk::fifo > > pool_type;

int serial_fib( int n)
{
	if( n < 2)
		return n;
	else
		return serial_fib( n - 1) + serial_fib( n - 2);
}

int parallel_fib_( int n, int cutof)
{
	if ( n < cutof)
	{
		if ( n == 0)
			boost::this_task::delay( pt::seconds( 2) );
		return serial_fib( n);
	}
	else
	{
		BOOST_ASSERT( boost::this_task::runs_in_pool() );
		tsk::task< int > t1(
			parallel_fib_,
			n - 1,
			cutof);
		tsk::task< int > t2(
			parallel_fib_,
			n - 2,
			cutof);
		tsk::handle< int > h1(
			tsk::async( boost::move( t1), tsk::as_sub_task() ) );
		tsk::handle< int > h2(
			tsk::async( boost::move( t2), tsk::as_sub_task() ) );
		return h1.get() + h2.get();
	}
}

void parallel_fib( int n)
{
	int result = parallel_fib_( n, 5);
	printf("fibonnaci(%d) == %d\n", n, result);
}

int main( int argc, char *argv[])
{
	try
	{
		pool_type pool( tsk::poolsize( 5) );
		
		for ( int i = 0; i < 10; ++i)
		{
			tsk::task< void > t(
				& parallel_fib,
				i);
			tsk::async(
				boost::move( t),
				pool);
		}

		return EXIT_SUCCESS;
	}
	catch ( std::exception const& e)
	{ std::cerr << "exception: " << e.what() << std::endl; }
	catch ( ... )
	{ std::cerr << "unhandled" << std::endl; }

	return EXIT_FAILURE;
}
