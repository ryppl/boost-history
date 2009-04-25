
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_ASYNC_H
#define BOOST_TASK_ASYNC_H

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <boost/task/default_pool.hpp>
#include <boost/task/detail/interrupter.hpp>
#include <boost/task/detail/thread_callable.hpp>
#include <boost/task/async_handle.hpp>
#include <boost/task/pool.hpp>
#include <boost/task/task.hpp>

namespace boost { namespace task
{
template< typename R >
async_handle< R > async_pool( task< R > t)
{ return get_default_pool().submit( t); }

template<
	typename R,
	typename Attr
>
async_handle< R > async_pool(
	task< R > t,
	Attr const& attr)
{ return get_default_pool().submit( t, attr); }

template<
	typename Channel,
	typename R
>
async_handle< R > async_pool(
	pool< Channel > & pool,
	task< R > t)
{ return pool.submit( t); }

template<
	typename Channel,
	typename R,
	typename Attr
>
async_handle< R > async_pool(
	pool< Channel > & pool,
	task< R > t,
	Attr const& attr)
{ return pool.submit( t, attr); }

namespace detail
{
struct joiner
{
	void operator()( thread * thrd)
	{
		thrd->join();
		delete thrd;
	}
};
}

template< typename R >
async_handle< R > async_thread( task< R > t)
{
	detail::interrupter intr;
	detail::thread_callable ca( t, intr);

	shared_ptr< thread > thrd( new thread( ca), detail::joiner() );
	ca.set( thrd);

	return async_handle< R >( t.get_id(), t.get_future(), intr);
}
} }

#endif // BOOST_TASK_ASYNC_H
