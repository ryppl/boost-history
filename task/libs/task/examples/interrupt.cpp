
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

inline
int fibonacci_fn( int n)
{
	if ( n == 0) return 0;
	if ( n == 1) return 1;
	int k1( 1), k2( 0);
	for ( int i( 2); i <= n; ++i)
	{
		boost::this_thread::interruption_point();
		int tmp( k1);
		k1 = k1 + k2;
		k2 = tmp;
	}
	boost::this_thread::interruption_point();
	return k1;
}

inline
void long_running_fn()
{ boost::this_thread::sleep( pt::seconds( 1) ); }

int main( int argc, char *argv[])
{
	try
	{
		pool_type pool( tsk::poolsize( 5) );
		
		tsk::task< void > t1( long_running_fn);
		tsk::task< int > t2( fibonacci_fn, 10);
		tsk::async(
			boost::move( t1),
			pool);
		std::cout << "poolsize == " << pool.size() << std::endl;
		std::cout << "idle threads == " << pool.idle() << std::endl;
		std::cout << "active threads == " << pool.active() << std::endl;
		tsk::handle< int > h(
			tsk::async(
				boost::move( t2),
				pool) );
		h.interrupt();
		std::cout << h.get() << std::endl;

		return EXIT_SUCCESS;
	}
	catch ( tsk::task_interrupted const& )
	{ std::cerr << "task_interrupted: task was interrupted" << std::endl; }
	catch ( std::exception const& e)
	{ std::cerr << "exception: " << e.what() << std::endl; }
	catch ( ... )
	{ std::cerr << "unhandled" << std::endl; }

	return EXIT_FAILURE;
}
