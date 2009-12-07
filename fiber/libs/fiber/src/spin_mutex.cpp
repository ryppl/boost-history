
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fiber/spin_mutex.hpp>

#include <boost/fiber/detail/atomic.hpp>
#include <boost/fiber/utility.hpp>

namespace boost {
namespace fibers {

spin_mutex::spin_mutex() :
	state_( 0)
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
			this_fiber::yield();	
	}
}

bool
spin_mutex::try_lock()
{
	uint32_t expected = 0;
	return detail::atomic_compare_exchange_strong( & state_, & expected, 1);
}

void
spin_mutex::unlock()
{ detail::atomic_exchange( & state_, 0); }

}}
