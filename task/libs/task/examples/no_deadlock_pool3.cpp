
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include "boost/task.hpp"

namespace tsk = boost::task;

typedef tsk::static_pool< tsk::unbounded_twolock_fifo > pool_type;

class event
{
private:
	class base : private boost::noncopyable
	{
	private:
		tsk::semaphore	sem_;

	public:
		base()
		: sem_( 0)
		{}

		void set()
		{ sem_.post(); }

		void wait()
		{
			if ( boost::this_task::runs_in_pool() )
			{
				while ( sem_.value() == 0)
					boost::this_task::block();
			}
			else
			{
				sem_.wait();
				sem_.post();
			}
		}
	};

	boost::shared_ptr< base >	impl_;

public:
	event()
	: impl_( new base)
	{}

	void set()
	{ impl_->set(); }

	void wait()
	{ impl_->wait(); }
};

void sub_task(
		int i,
		int n,
		event inner_ev)
{
	BOOST_ASSERT( boost::this_task::runs_in_pool() );

	fprintf( stderr, "t%d running ...\n", i);

	if ( i == n - 1)
			inner_ev.set();
	else
			inner_ev.wait();

	fprintf( stderr, "t%d finished ...\n", i);
}

void main_task(
		pool_type & pool,
		int n,
		event outer_ev)
{
	BOOST_ASSERT( boost::this_task::runs_in_pool() );

	fprintf( stderr, "main-task running %d sub-tasks\n", n);

	event inner_ev;

	for ( int i = 0; i < n; ++i)
		tsk::async(
				tsk::make_task(
					& sub_task,
					i,
					n,
					inner_ev),
				tsk::as_sub_task() );

	inner_ev.wait();
	outer_ev.set();
}

int main( int argc, char *argv[])
{
	try
	{
		tsk::poolsize psize( boost::thread::hardware_concurrency() );
		pool_type pool( psize);

		int n = 32;	
		event outer_ev;
		tsk::async(
			tsk::make_task(
				& main_task,
				boost::ref( pool),
				n,
				outer_ev),
			pool);

		fprintf( stderr, "main thread: waiting for t0 to finish\n");
		outer_ev.wait();
		fprintf( stderr, "main thread: t0 finished\n");

		return EXIT_SUCCESS;
	}
	catch ( std::exception const& e)
	{ std::cerr << "exception: " << e.what() << std::endl; }
	catch ( ... )
	{ std::cerr << "unhandled" << std::endl; }

	return EXIT_FAILURE;
}
