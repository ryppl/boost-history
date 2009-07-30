
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)txt)

#ifndef BOOST_TASK_UTILITY_H
#define BOOST_TASK_UTILITY_H

#include <boost/assert.hpp>
#include <boost/thread.hpp>
#include <boost/thread/thread_time.hpp>

#include <boost/task/detail/worker.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace this_task
{
template< typename Pool >
Pool & get_pool()
{
	task::detail::worker * w( task::detail::worker::tss_get() );
	BOOST_ASSERT( w);
	return w->get_pool< Pool >();
}

inline
bool runs_in_pool()
{ return task::detail::worker::tss_get() != 0; }

inline
bool block()
{
	task::detail::worker * w( task::detail::worker::tss_get() );
	BOOST_ASSERT( w);
	return w->block();
}

inline
thread::id worker_id()
{
	task::detail::worker * w( task::detail::worker::tss_get() );
	BOOST_ASSERT( w);
	return w->get_id();
}
}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_UTILITY_H
