
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/spin_count_down_event.hpp"

#include <boost/thread.hpp>

#include <boost/task/detail/atomic.hpp>
#include <boost/task/spin_mutex.hpp>
#include <boost/task/utility.hpp>

namespace boost {
namespace task {

spin_count_down_event::spin_count_down_event( uint32_t initial) :
	initial_( initial),
	current_( initial_)
{}

uint32_t
spin_count_down_event::initial() const
{ return initial_; }

uint32_t
spin_count_down_event::current() const
{ return detail::atomic_load( & current_); }

bool
spin_count_down_event::is_set() const
{ return 0 == detail::atomic_load( & current_); }

void
spin_count_down_event::set()
{
	for (;;)
	{
		if ( 0 == detail::atomic_load( & current_) )
			return;
		uint32_t expected = current_;
		if ( detail::atomic_compare_exchange_strong( & current_, & expected, expected - 1) )
			return;
	}
}

void
spin_count_down_event::wait()
{
	while ( 0 != detail::atomic_load( & current_) )
	{
		this_thread::interruption_point();
		if ( this_task::runs_in_pool() )
			this_task::block();
		else
			this_thread::yield();	
		this_thread::interruption_point();
	}
}

bool
spin_count_down_event::wait( system_time const& abs_time)
{
	if ( get_system_time() >= abs_time) return false;

	while ( 0 < detail::atomic_load( & current_) )
	{
		this_thread::interruption_point();
		if ( this_task::runs_in_pool() )
			this_task::block();
		else
			this_thread::yield();
		this_thread::interruption_point();

		if ( get_system_time() >= abs_time) return false;
	}

	return true;
}

}}
