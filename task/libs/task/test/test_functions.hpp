//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_TEST_FUNCTIONS_H
#define BOOST_TP_TEST_FUNCTIONS_H

#include <cstdlib>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/function.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/thread.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/utility.hpp>

#include <boost/task.hpp>

namespace pt = boost::posix_time;
namespace tsk = boost::task;

void barrier_fn(
	boost::barrier & b)
{ b.wait(); }

void delay_fn( pt::time_duration const& td)
{ boost::this_thread::sleep( td); }

inline
int fibonacci_fn( int n)
{
	if ( n < 2) return n;
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

inline
void buffer_fibonacci_fn(
	std::vector< int > & buffer,
	int n)
{ buffer.push_back( fibonacci_fn( n) ); }

inline
bool runs_in_pool_fn()
{ return boost::this_task::runs_in_pool(); }

inline
void throwing_fn()
{ throw std::runtime_error("exception thrown"); }

#endif // BOOST_TP_TEST_FUNCTIONS_H
