#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <vector>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/ref.hpp>

#include "boost/task.hpp"

namespace pt = boost::posix_time;
namespace tsk = boost::task;

inline
int fibonacci_fn( int n)
{
	boost::this_thread::sleep( pt::milliseconds( 500) );
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

int main( int argc, char *argv[])
{
	try
	{
		tsk::pool<
			tsk::unbounded_channel< tsk::fifo >
		> pool( tsk::poolsize( 1) );

		tsk::task< int > t(
			tsk::make_task(
				fibonacci_fn,
				10) );
		tsk::launch_in_pool( pool, t);

		boost::this_thread::sleep( pt::milliseconds( 250) );

		pool.shutdown_now();

		std::cout << t.get() << std::endl;

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
