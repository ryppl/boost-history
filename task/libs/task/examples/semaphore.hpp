
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_EXAMPLES_SEMAPHORE_H
#define BOOST_TASK_EXAMPLES_SEMAPHORE_H

#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include <boost/bind.hpp>
#include <boost/ref.hpp>

#include "boost/task.hpp"

namespace tsk = boost::task;

inline
void loop_fn( int n)
{
	for ( int i = 0; i < n; ++i)
	{
		printf("task 3: %d-th iteration\n", i);
		boost::this_task::block();
	}
	printf("task 3: end\n");
}

inline
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

inline
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

#endif // BOOST_TASK_EXAMPLES_SEMAPHORE_H
