#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <vector>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/thread.hpp>

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
	boost::shared_future< void >		f_;
	int								offset_;

	class holder
	{
	private:
		boost::shared_future< void >		f_;

	public:
		holder( boost::shared_future< void > const& f)
		: f_( f)
		{}

		bool operator()()
		{ return f_.is_ready(); }
	};

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
			if ( n == 7)
			{
				holder hldr( f_);
				boost::this_task::reschedule_until( hldr);
			}

			boost::function< int() > fn1 = boost::bind(
						& fibo::par_,
						* this,
						n - 1);
			tp::task< int > t1(
				boost::this_task::get_thread_pool< pool_type >().submit(
					fn1) );
			boost::function< int() > fn2 = boost::bind(
						& fibo::par_,
						* this,
						n - 2);
			tp::task< int > t2(
				boost::this_task::get_thread_pool< pool_type >().submit(
					fn2) );

			return t1.result().get() + t2.result().get();
		}
	}

public:
	fibo(
		boost::shared_future< void > f,
		int offset)
	:  f_( f), offset_( offset)
	{}

	int execute( int n)
	{
		int result( par_( n) );
		return result;
	}
};

void f() {}

int main( int argc, char *argv[])
{
	try
	{
		pool_type pool( tp::poolsize( 1) );
		boost::packaged_task< void > tsk( boost::bind( f) );
		boost::shared_future< void > f( tsk.get_future() );
		fibo fib( f, 3);
		std::vector< tp::task< int > > results;
		results.reserve( 40);

		pt::ptime start( pt::microsec_clock::universal_time() );

		for ( int i = 0; i < 10; ++i)
			results.push_back(
				pool.submit(
					boost::bind(
						& fibo::execute,
						boost::ref( fib),
						i) ) );

		::sleep( 1);
		tsk();

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
