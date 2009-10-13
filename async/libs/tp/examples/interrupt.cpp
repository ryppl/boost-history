#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <vector>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "boost/tp/fifo.hpp"
#include "boost/tp/pool.hpp"
#include "boost/tp/poolsize.hpp"
#include "boost/tp/unbounded_channel.hpp"

namespace pt = boost::posix_time;
namespace tp = boost::tp;

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
		tp::pool<
			tp::unbounded_channel< tp::fifo >
		> pool( tp::poolsize( 1) );
		pool.submit(
			boost::bind(
				long_running_fn) );
		std::cout << "poolsize == " << pool.size() << std::endl;
		std::cout << "idle threads == " << pool.idle() << std::endl;
		std::cout << "active threads == " << pool.active() << std::endl;
		tp::task< int > t(
			pool.submit(
				boost::bind(
					fibonacci_fn,
					10) ) );
		t.interrupt();
		std::cout << t.result().get() << std::endl;

		return EXIT_SUCCESS;
	}
	catch ( boost::thread_interrupted const& )
	{ std::cerr << "thread_interrupted: task was canceled" << std::endl; }
	catch ( std::exception const& e)
	{ std::cerr << "exception: " << e.what() << std::endl; }
	catch ( ... )
	{ std::cerr << "unhandled" << std::endl; }

	return EXIT_FAILURE;
}
