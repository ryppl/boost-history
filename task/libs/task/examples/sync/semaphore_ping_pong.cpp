
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include <boost/bind.hpp>
#include <boost/ref.hpp>

#include "boost/task.hpp"

namespace tsk = boost::task;

void start_with_sem_a(
		int n,
		tsk::semaphore & sem_a,
		tsk::semaphore & sem_b)
{
	for ( int i = 0; i < n; ++i)
	{
		printf("task 1: %d-th sema_a.post()\n", i);
		sem_a.post();
		printf("task 1: %d-th sema_b.wait()\n", i);
		sem_b.wait();
	}
	printf("task 1: end\n");
}

void start_with_sem_b(
		int n,
		tsk::semaphore & sem_a,
		tsk::semaphore & sem_b)
{
	for ( int i = 0; i < n; ++i)
	{
		printf("task 2: %d-th sema_b.post()\n", i);
		sem_b.post();
		printf("task 2: %d-th sema_a.wait()\n", i);
		sem_a.wait();
	}
	printf("task 2: end\n");
}

void thread_only()
{
	int n = 10;	
	tsk::semaphore sem_a( 0), sem_b( 0);	
	tsk::handle< void > h1 = tsk::async(
		tsk::make_task(
			& start_with_sem_a,
			n,
			boost::ref( sem_a),
			boost::ref( sem_b) ),
		tsk::new_thread() );
	tsk::handle< void > h2 = tsk::async(
		tsk::make_task(
			& start_with_sem_b,
			n,
			boost::ref( sem_a),
			boost::ref( sem_b) ),
		tsk::new_thread() );

	tsk::waitfor_all( h1, h2);
}

void pool_only()
{
	tsk::static_pool< tsk::unbounded_twolock_fifo > pool( tsk::poolsize( 1) );
	
	int n = 10;	
	tsk::semaphore sem_a( 0), sem_b( 0);	
	tsk::async(
		tsk::make_task(
			& start_with_sem_a,
			n,
			boost::ref( sem_a),
			boost::ref( sem_b) ),
		pool);
	tsk::async(
		tsk::make_task(
			& start_with_sem_b,
			n,
			boost::ref( sem_a),
			boost::ref( sem_b) ),
		pool);
}

void pool_and_thread()
{
	tsk::static_pool< tsk::unbounded_twolock_fifo > pool( tsk::poolsize( 1) );
	
	int n = 10;	
	tsk::semaphore sem_a( 0), sem_b( 0);	
	tsk::async(
		tsk::make_task(
			& start_with_sem_a,
			n,
			boost::ref( sem_a),
			boost::ref( sem_b) ),
		pool);
	tsk::handle< void > h = tsk::async(
		tsk::make_task(
			& start_with_sem_b,
			n,
			boost::ref( sem_a),
			boost::ref( sem_b) ),
		tsk::new_thread() );

	h.wait();
}

int main( int argc, char *argv[])
{
	try
	{
		pool_only();
		thread_only();
		pool_and_thread();
		return EXIT_SUCCESS;
	}
	catch ( std::exception const& e)
	{ std::cerr << "exception: " << e.what() << std::endl; }
	catch ( ... )
	{ std::cerr << "unhandled" << std::endl; }

	return EXIT_FAILURE;
}
