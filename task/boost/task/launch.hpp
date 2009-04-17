//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_LAUNCH_H
#define BOOST_TASK_LAUNCH_H

#include <boost/task/default_pool.hpp>
#include <boost/task/pool.hpp>
#include <boost/task/task.hpp>

namespace boost { namespace task
{
template< typename R >
void launch_in_pool( task< R > t)
{ get_default_pool().submit( t); }

template<
	typename R,
	typename Attr
>
void launch_in_pool(
	task< R > t,
	Attr const& attr)
{ get_default_pool().submit( t, attr); }

template<
	typename Channel,
	typename R
>
void launch_in_pool(
	pool< Channel > & pool,
	task< R > t)
{ pool.submit( t); }

template<
	typename Channel,
	typename R,
	typename Attr
>
void launch_in_pool(
	pool< Channel > & pool,
	task< R > t,
	Attr const& attr)
{ pool.submit( t, attr); }

template< typename R >
void launch_in_thread( task< R > t)
{
	thread th( t);
	th.join();
}

template< typename R >
void launch_in_current( task< R > t)
{ t(); }
} }

#endif // BOOST_TASK_LAUNCH_H
