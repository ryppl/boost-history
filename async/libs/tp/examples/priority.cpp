#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <string>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "boost/tp/priority.hpp"
#include "boost/tp/pool.hpp"
#include "boost/tp/poolsize.hpp"
#include "boost/tp/unbounded_channel.hpp"

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
		pool.submit(
			boost::bind(
				long_running_fn),
			0);
		pool.submit(
			boost::bind(
				print_fn,
				"This"),
			0);
		pool.submit(
			boost::bind(
				print_fn,
				"a text.\n"),
			2);
		pool.submit(
			boost::bind(
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
