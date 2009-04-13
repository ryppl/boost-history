#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <vector>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "boost/tp.hpp"

namespace pt = boost::posix_time;
namespace tp = boost::tp;

typedef tp::default_pool pool_type;

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
		if ( n == 7)
			boost::this_task::yield();

		if ( n < cutof_)
			return serial_fib( n);
		else
		{
			tp::task< long > t1(
				boost::this_task::get_thread_pool< pool_type >().submit(
					boost::bind(
						& fib_task::execute,
						boost::ref( * this),
						n - 1) ) );
			tp::task< long > t2(
				boost::this_task::get_thread_pool< pool_type >().submit(
					boost::bind(
						& fib_task::execute,
						boost::ref( * this),
						n - 2) ) );
			return t1.get() + t2.get();
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
			tp::get_default_pool().submit(
				boost::bind(
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
