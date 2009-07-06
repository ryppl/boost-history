
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_AS_SUB_TASK_H
#define BOOST_TASK_AS_SUB_TASK_H

#include <boost/bind.hpp>
#include <boost/config.hpp>
#include <boost/function.hpp>
#include <boost/thread/detail/move.hpp>

#include <boost/task/context.hpp>
#include <boost/task/detail/worker.hpp>
#include <boost/task/future.hpp>
#include <boost/task/handle.hpp>
#include <boost/task/new_thread.hpp>
#include <boost/task/task.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{

struct as_sub_task
{
	template< typename R >
	handle< R > operator()( task< R > t)
	{
		detail::worker * w( detail::worker::tss_get() );
		if ( w)
		{
			shared_future< R > f( t.get_future() );
			function< bool() > wcb(
				bind(
					& shared_future< R >::is_ready,
					f) );
			t.set_wait_callback(
				bind(
					( void ( detail::worker::*)( function< bool() > const&) ) & detail::worker::reschedule_until,
					w,
					wcb) );
			context ctx;
			handle< R > h( ctx.get_handle( f) );
			w->put( ctx.get_callable( boost::move( t) ) );
			return h;
		}
		else
			return new_thread()( boost::move( t) );
	}
};

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_AS_SUB_TASK_H
