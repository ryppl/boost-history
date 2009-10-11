
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/spin_manual_reset_event.hpp"

#include <boost/assert.hpp>
#include <boost/thread.hpp>

#include <boost/task/detail/atomic.hpp>
#include <boost/task/spin_lock.hpp>
#include <boost/task/utility.hpp>

namespace boost {
namespace task {

spin_manual_reset_event::spin_manual_reset_event( bool isset)
:
state_( isset ? SET : RESET),
waiters_( 0),
enter_mtx_()
{}

void
spin_manual_reset_event::set()
{
	enter_mtx_.lock();

	state_t expected = RESET;
	if ( ! detail::atomic_compare_exchange_strong(
			static_cast< uint32_t volatile* >( & state_),
			static_cast< uint32_t * >( & expected),
			static_cast< uint32_t >( SET) ) ||
		! detail::atomic_load( static_cast< uint32_t volatile* >( & waiters_) ) )
		enter_mtx_.unlock();
}

void
spin_manual_reset_event::reset()
{
	spin_lock< spin_mutex >	lk( enter_mtx_);
	BOOST_ASSERT( lk);

	detail::atomic_exchange(
		static_cast< uint32_t volatile* >( & state_),
		static_cast< uint32_t >( RESET) );
}

void
spin_manual_reset_event::wait()
{
	{
		spin_lock< spin_mutex > lk( enter_mtx_);
		BOOST_ASSERT( lk);
		detail::atomic_fetch_add( & waiters_, 1);
	}

	while ( RESET == detail::atomic_load( static_cast< uint32_t volatile* >( & state_) ) )
	{
		if ( this_task::runs_in_pool() )
			this_task::block();
		else
			this_thread::yield();	
	}

	if ( 1 == detail::atomic_fetch_sub( & waiters_, 1) )
		enter_mtx_.unlock();
}

bool
spin_manual_reset_event::wait( system_time const& abs_time)
{
	if ( get_system_time() >= abs_time) return false;

	while ( RESET == detail::atomic_load( & state_) )
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
