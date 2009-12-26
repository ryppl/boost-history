
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
namespace tsk = boost::tasks;

typedef tsk::static_pool< tsk::unbounded_twolock_fifo > pool_type;

int serial_fib( int n)
{
	if( n < 2)
		return n;
	else
		return serial_fib( n - 1) + serial_fib( n - 2);
}

int parallel_fib( int n, int	cutof)
{
	if ( n < cutof) return serial_fib( n);
	else
	{
		BOOST_ASSERT( boost::this_task::runs_in_pool() );
		tsk::task< int > t1(
			parallel_fib,
			n - 1,
			cutof);
		tsk::task< int > t2(
			parallel_fib,
			n - 2,
			cutof);
		tsk::handle< int > h1(
			tsk::async(
				boost::move( t1),
				tsk::as_sub_task() ) ) ;
		tsk::handle< int > h2(
			tsk::async(
				boost::move( t2),
				tsk::as_sub_task() ) );
		return h1.get() + h2.get();
	}
}

int main( int argc, char *argv[])
{
	try
	{
		pool_type pool( tsk::poolsize( 5) );

		std::vector< tsk::handle< int > > results;
		results.reserve( 10);

		pt::ptime start( pt::microsec_clock::universal_time() );

		for ( int i = 0; i < 10; ++i)
		{
			tsk::task< int > t(
				& parallel_fib,
				i,
				5);
			results.push_back(
				tsk::async(
					boost::move( t),
					pool) );
		}

		tsk::waitfor_all( results.begin(), results.end() );

		int k = 0;
		std::vector< tsk::handle< int > >::iterator e( results.end() );
		for (
			std::vector< tsk::handle< int > >::iterator i( results.begin() );
			i != e;
			++i)
			std::cout << "fibonacci(" << k++ << ") == " << i->get() << std::endl;

		pt::ptime stop( pt::microsec_clock::universal_time() );
		std::cout << ( stop - start).total_milliseconds() << " milli seconds" << std::endl;

		return EXIT_SUCCESS;
	}
	catch ( std::exception const& e)
	{ std::cerr << "exception: " << e.what() << std::endl; }
	catch ( ... )
	{ std::cerr << "unhandled" << std::endl; }

	return EXIT_FAILURE;
}
