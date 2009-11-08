
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/fiber/mutex.hpp"

#include "boost/fiber/detail/atomic.hpp"
#include "boost/fiber/utility.hpp"

namespace boost {
namespace fiber {

mutex::mutex() :
	state_( 0)
{}

void
mutex::lock()
{
	for (;;)
	{
		uint32_t expected = 0;
		if ( detail::atomic_compare_exchange_strong( & state_, & expected, 1) )
			break;
		else
		{
			this_fiber::interruption_point();
			this_fiber::yield();	
			this_fiber::interruption_point();
		}	
	}
}

bool
mutex::try_lock()
{
	uint32_t expected = 0;
	return detail::atomic_compare_exchange_strong( & state_, & expected, 1);
}

bool
mutex::timed_lock( system_time const& abs_time)
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

		this_fiber::interruption_point();
		this_fiber::yield();	
		this_fiber::interruption_point();
	}

	return true;
}

void
mutex::unlock()
{
	uint32_t expected = 1;
	detail::atomic_compare_exchange_strong( & state_, & expected, 0);
}

}}
