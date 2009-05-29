
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_ASYNC_H
#define BOOST_TASK_ASYNC_H

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <boost/task/default_pool.hpp>
#include <boost/task/detail/interrupter.hpp>
#include <boost/task/detail/pool_callable.hpp>
#include <boost/task/detail/thread_callable.hpp>
#include <boost/task/detail/worker.hpp>
#include <boost/task/handle.hpp>
#include <boost/task/future.hpp>
#include <boost/task/static_pool.hpp>
#include <boost/task/task.hpp>

namespace boost { namespace task
{

namespace detail
{
struct joiner
{
	void operator()( thread * thrd)
	{
		try
		{ thrd->join(); }
		catch (...)
		{}
		delete thrd;
	}
};
}

struct own_thread
{
	template< typename R >
	handle< R > operator()( task< R > t)
	{
		t();
		detail::interrupter intr;
		intr.reset();
		return handle< R >(
			t.get_id(),
			t.get_future(),
			intr);
	}
};

struct new_thread
{
	template< typename R >
	handle< R > operator()( task< R > t)
	{
		detail::interrupter intr;
		detail::thread_callable ca( t, intr);
	
		shared_ptr< thread > thrd( new thread( ca), detail::joiner() );
		intr.set( thrd);
	
		return handle< R >( t.get_id(), t.get_future(), intr);
	}
};

struct as_sub_task
{
	template< typename R >
	handle< R > operator()( task< R > t)
	{
		detail::worker * w( detail::worker::tss_get() );
		if ( w)
		{
			detail::interrupter intr;
			shared_future< R > fut( t.get_future() );
			function< bool() > wcb(
				bind(
					& shared_future< R >::is_ready,
					fut) );
			t.set_wait_callback(
				bind(
					( void ( detail::worker::*)( function< bool() > const&) ) & detail::worker::reschedule_until,
					w,
					wcb) );
			w->put( detail::pool_callable( t, intr) );
			return handle< R >( t.get_id(), fut, intr);
		}
		else
			return new_thread()( t);
	}
};

template< typename AE, typename R >
handle< R > async( AE ae, task< R > t)
{ return ae( t); }

template< typename R, typename Channel >
handle< R > async( static_pool< Channel > & pool, task< R > t)
{ return pool.submit( t); }

template< typename R, typename Channel, typename Attr >
handle< R > async( static_pool< Channel > & pool, task< R > t, Attr attr)
{ return pool.submit( t, attr); }

} }

#endif // BOOST_TASK_ASYNC_H
