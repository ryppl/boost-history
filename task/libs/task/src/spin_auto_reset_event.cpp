
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/spin_auto_reset_event.hpp"

#include <boost/thread.hpp>

#include <boost/task/detail/atomic.hpp>
#include <boost/task/utility.hpp>

namespace boost {
namespace task {

spin_auto_reset_event::spin_auto_reset_event( bool isset)
: state_( isset ? SET : RESET)
{}

void
spin_auto_reset_event::set()
{
	detail::atomic_exchange(
			static_cast< uint32_t volatile* >( & state_),
			static_cast< uint32_t >( SET) );
}

void
spin_auto_reset_event::wait()
{
	state_t expected = SET;
	while ( ! detail::atomic_compare_exchange_strong(
			static_cast< uint32_t volatile* >( & state_),
			static_cast< uint32_t * >( & expected),
			static_cast< uint32_t >( RESET) ) )
	{
		if ( this_task::runs_in_pool() )
			this_task::block();
		else
			this_thread::yield();	
	}
}

bool
spin_auto_reset_event::wait( system_time const& abs_time)
{
	if ( get_system_time() >= abs_time) return false;

	state_t expected = SET;
	while ( ! detail::atomic_compare_exchange_strong(
			static_cast< uint32_t volatile* >( & state_),
			static_cast< uint32_t * >( & expected),
			static_cast< uint32_t >( RESET) ) )
	{
		if ( this_task::runs_in_pool() )
			this_task::block();
		else
			this_thread::yield();	
	
		if ( get_system_time() >= abs_time) return false;
	}

	return true;
}

}}
