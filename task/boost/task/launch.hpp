
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_LAUNCH_H
#define BOOST_TASK_LAUNCH_H

#include <boost/task/default_pool.hpp>
#include <boost/task/handle.hpp>
#include <boost/task/pool.hpp>
#include <boost/task/task.hpp>

namespace boost { namespace task
{
template< typename R >
handle< R > launch( task< R > t)
{ return get_default_pool().submit( t); }

template<
	typename R,
	typename Attr
>
handle< R > launch(
	task< R > t,
	Attr const& attr)
{ return get_default_pool().submit( t, attr); }

template<
	typename Channel,
	typename R
>
handle< R > launch(
	pool< Channel > & pool,
	task< R > t)
{ return pool.submit( t); }

template<
	typename Channel,
	typename R,
	typename Attr
>
handle< R > launch(
	pool< Channel > & pool,
	task< R > t,
	Attr const& attr)
{ return pool.submit( t, attr); }
} }

#endif // BOOST_TASK_LAUNCH_H
