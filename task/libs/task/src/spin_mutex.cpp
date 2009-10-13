
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/spin_mutex.hpp"

#include <boost/thread.hpp>

#include "boost/task/detail/atomic.hpp"
#include "boost/task/utility.hpp"

namespace boost {
namespace task {

spin_mutex::spin_mutex()
: state_( 0)
{}

void
spin_mutex::lock()
{
	for (;;)
	{
		uint32_t expected = 0;
		if ( detail::atomic_compare_exchange_strong( & state_, & expected, 1) )
			break;
		else
		{
			this_thread::interruption_point();
			if ( this_task::runs_in_pool() )
				this_task::block();
			else
				this_thread::yield();	
			this_thread::interruption_point();
		}	
	}
}

bool
spin_mutex::try_lock()
{
	uint32_t expected = 0;
	return detail::atomic_compare_exchange_strong( & state_, & expected, 1);
}

bool
spin_mutex::timed_lock( system_time const& abs_time)
{
	if ( abs_time.is_infinity() )
	{
		lock();
		return true;
	}

	if ( get_system_time() >= abs_time)
		return false;

	for (;;)
	{
		if ( try_lock() ) break;

		if ( get_system_time() >= abs_time)
			return false;

		this_thread::interruption_point();
		if ( this_task::runs_in_pool() )
			this_task::block();
		else
			this_thread::yield();	
		this_thread::interruption_point();
	}

	return true;
}

void
spin_mutex::unlock()
{
	uint32_t expected = 1;
	detail::atomic_compare_exchange_strong( & state_, & expected, 0);
}

}}
