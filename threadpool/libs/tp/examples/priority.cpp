#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <string>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "boost/tp.hpp"

namespace pt = boost::posix_time;
namespace tp = boost::tp;

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
		tp::pool<
			tp::unbounded_channel< tp::priority< int > >
		> pool( tp::poolsize( 1) );

		tp::task< void > t1(
			boost::bind(
				long_running_fn) );
		tp::task< void > t2(
			boost::bind(
				print_fn,
				"This") );
		tp::task< void > t3(
			boost::bind(
				print_fn,
				"a text.\n") );
		tp::task< void > t4(
			boost::bind(
				print_fn,
				" is ") );

		tp::launch_in_pool( pool, t1, 0);
		tp::launch_in_pool( pool, t2, 0);
		tp::launch_in_pool( pool, t3, 2);
		tp::launch_in_pool( pool, t4, 1);

		return EXIT_SUCCESS;
	}
	catch ( std::exception const& e)
	{ std::cerr << "exception: " << e.what() << std::endl; }
	catch ( ... )
	{ std::cerr << "unhandled" << std::endl; }

	return EXIT_FAILURE;
}
