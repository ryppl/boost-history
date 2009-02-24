#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <string>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "boost/tp/smart.hpp"
#include "boost/tp/pool.hpp"
#include "boost/tp/poolsize.hpp"
#include "boost/tp/unbounded_channel.hpp"

namespace pt = boost::posix_time;
namespace tp = boost::tp;

inline
void fibonacci_fn( int n)
{
	if ( n == 0)
	{
		printf("fibonacci(%d) == 0\n", n);
		return;
	}
	if ( n == 1)
	{
		printf("fibonacci(%d) == 1\n", n);
		return;
	}
	int k1( 1), k2( 0);
	for ( int i( 2); i <= n; ++i)
	{
		int tmp( k1);
		k1 = k1 + k2;
		k2 = tmp;
	}
	printf("fibonacci(%d) == %d\n", n, k1);
}

inline
void long_running_fn()
{ boost::this_thread::sleep( pt::milliseconds( 500) ); }

int main( int argc, char *argv[])
{
	try
	{
		tp::pool<
			tp::unbounded_channel<
				tp::smart<
					int,
					std::less< int >,
					tp::replace_oldest,
					tp::take_oldest
				>
			>
		> pool( tp::poolsize( 1) );
		pool.submit(
			boost::bind(
				long_running_fn),
			0);
		pool.submit(
			boost::bind(
				fibonacci_fn,
				0),
			1);
		pool.submit(
			boost::bind(
				fibonacci_fn,
				1),
			2);
		pool.submit(
			boost::bind(
				fibonacci_fn,
				10),
			2);

		return EXIT_SUCCESS;
	}
	catch ( std::exception const& e)
	{ std::cerr << "exception: " << e.what() << std::endl; }
	catch ( ... )
	{ std::cerr << "unhandled" << std::endl; }

	return EXIT_FAILURE;
}
