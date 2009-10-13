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
#include <boost/future.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/thread.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/utility.hpp>

#include <boost/tp/bounded_channel.hpp>
#include <boost/tp/exceptions.hpp>
#include <boost/tp/fifo.hpp>
#include <boost/tp/lifo.hpp>
#include <boost/tp/pool.hpp>
#include <boost/tp/poolsize.hpp>
#include <boost/tp/priority.hpp>
#include <boost/tp/smart.hpp>
#include <boost/tp/task.hpp>
#include <boost/tp/unbounded_channel.hpp>
#include <boost/tp/watermark.hpp>

#include <boost/tp/detail/interrupter.hpp>

namespace pt = boost::posix_time;
namespace tp = boost::tp;

inline
int add_fn(
	int a,
	int b)
{
	boost::this_thread::interruption_point();
	return a + b;
}

template< typename Fn >
typename boost::result_of< Fn() >::type barrier_fn(
	Fn const& fn,
	boost::barrier & b)
{
	b.wait();
	return fn();
}

template< typename Fn >
typename boost::result_of< Fn() >::type delay_fn(
	Fn const& fn,
	pt::time_duration const& td)
{
	boost::this_thread::sleep( td);
	return fn();
}

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
void throwing_fn()
{ throw std::runtime_error("exception thrown"); }

#endif // BOOST_TP_TEST_FUNCTIONS_H
