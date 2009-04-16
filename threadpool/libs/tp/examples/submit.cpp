#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include <boost/bind.hpp>

#include "boost/tp.hpp"

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

int main( int argc, char *argv[])
{
	try
	{
		tp::task< int > t(
				boost::bind(
					fibonacci_fn,
					10) );
// 		tp::launch_in_pool( t);
// 		tp::launch_in_thread( t);
		tp::launch_local( t);
		std::cout << t.get() << std::endl;

		return EXIT_SUCCESS;
	}
	catch ( std::exception const& e)
	{ std::cerr << "exception: " << e.what() << std::endl; }
	catch ( ... )
	{ std::cerr << "unhandled" << std::endl; }

	return EXIT_FAILURE;
}
