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

typedef tp::pool< tp::unbounded_channel< tp::fifo > > pool_type;

class fibo
{
private:
	int		offset_;

	int seq_( int n)
	{
		if ( n <= 1) return n;
		else return seq_( n - 2) + seq_( n - 1);
	}
	
	int par_( int n)
	{
		if ( n <= offset_) return seq_( n);
		else
		{
			tp::task< int > t1(
				boost::this_task::get_thread_pool< pool_type >().submit(
					boost::bind(
						& fibo::par_,
						boost::ref( * this),
						n - 1) ) );
			tp::task< int > t2(
				boost::this_task::get_thread_pool< pool_type >().submit(
					boost::bind(
						& fibo::par_,
						boost::ref( * this),
						n - 2) ) );
			return t1.result().get() + t2.result().get();
		}
	}

public:
	fibo( int offset)
	: offset_( offset)
	{}

	int execute( int n)
	{
		int result( par_( n) );
		return result;
	}
};

int main( int argc, char *argv[])
{
	try
	{
		pool_type pool( tp::poolsize( 3) );
		fibo fib( 5);
		std::vector< tp::task< int > > results;
		results.reserve( 40);

		pt::ptime start( pt::microsec_clock::universal_time() );

		for ( int i = 0; i < 32; ++i)
			results.push_back(
				pool.submit(
					boost::bind(
						& fibo::execute,
						boost::ref( fib),
						i) ) );

		int k = 0;
		std::vector< tp::task< int > >::iterator e( results.end() );
		for (
			std::vector< tp::task< int > >::iterator i( results.begin() );
			i != e;
			++i)
			std::cout << "fibonacci " << k++ << " == " << i->result().get() << std::endl;

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
