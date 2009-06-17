
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

#include <boost/task/detail/interrupter.hpp>
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
# if defined(BOOST_HAS_RVALUE_REFS)
	handle< R > operator()( task< R > && t_)
# else
	handle< R > operator()( boost::detail::thread_move_t< task< R > > t_)
# endif
	{
		detail::worker * w( detail::worker::tss_get() );
		if ( w)
		{
			task< R > t( t_);
			shared_future< R > fut( t.get_future() );
			detail::interrupter intr;
			function< bool() > wcb(
				bind(
					& shared_future< R >::is_ready,
					fut) );
			t.set_wait_callback(
				bind(
					( void ( detail::worker::*)( function< bool() > const&) ) & detail::worker::reschedule_until,
					w,
					wcb) );
			w->put( detail::pool_callable( boost::move( t), intr) );
			return handle< R >( fut, intr);
		}
		else
			return new_thread()( t_);
	}
};
} }

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_AS_SUB_TASK_H
