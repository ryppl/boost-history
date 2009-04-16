//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_LAUNCH_H
#define BOOST_TP_LAUNCH_H

#include <boost/tp/default_pool.hpp>
#include <boost/tp/pool.hpp>
#include <boost/tp/task.hpp>

namespace boost { namespace tp
{
template< typename R >
void launch_in_pool( task< R > tsk)
{ get_default_pool().submit( tsk); }

template<
	typename R,
	typename Attr
>
void launch_in_pool(
	task< R > tsk,
	Attr const& attr)
{ get_default_pool().submit( tsk, attr); }

template<
	typename Channel,
	typename R
>
void launch_in_pool(
	pool< Channel > & pool,
	task< R > tsk)
{ pool.submit( tsk); }

template<
	typename Channel,
	typename R,
	typename Attr
>
void launch_in_pool(
	pool< Channel > & pool,
	task< R > tsk,
	Attr const& attr)
{ pool.submit( tsk, attr); }

template< typename R >
void launch_in_thread( task< R > tsk)
{
	thread t( tsk);
	t.join();
}

template< typename R >
void launch_in_current( task< R > tsk)
{ tsk(); }
} }

#endif // BOOST_TP_LAUNCH_H
