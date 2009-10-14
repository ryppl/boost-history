
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/spin_condition.hpp"

#include <boost/assert.hpp>
#include <boost/thread.hpp>

#include <boost/task/detail/atomic.hpp>
#include <boost/task/spin_mutex.hpp>
#include <boost/task/utility.hpp>

namespace boost {
namespace task {

void
spin_condition::notify_( uint32_t cmd)
{
	enter_mtx_.lock();

	if ( 0 == detail::atomic_load( & waiters_) )
	{
		enter_mtx_.unlock();
		return;
	}

	uint32_t expected = static_cast< uint32_t >( SLEEPING);
	while ( ! detail::atomic_compare_exchange_strong(
				& cmd_, & expected, cmd) )
	{
		this_thread::interruption_point();
		if ( this_task::runs_in_pool() )
			this_task::block();
		else
			this_thread::yield();	
		this_thread::interruption_point();
	}
}

void
spin_condition::wait_( spin_mutex & mtx)
{
	{
		spin_mutex::scoped_lock lk( enter_mtx_);
		BOOST_ASSERT( lk);
		detail::atomic_fetch_add( & waiters_, 1);
		mtx.unlock();
	}

	bool unlock_enter_mtx = false;
	for (;;)
	{
		while ( static_cast< uint32_t >( SLEEPING) == detail::atomic_load( & cmd_) )
		{
			this_thread::interruption_point();
			if ( this_task::runs_in_pool() )
				this_task::block();
			else
				this_thread::yield();	
			this_thread::interruption_point();
		}

		spin_mutex::scoped_lock lk( check_mtx_);
		BOOST_ASSERT( lk);

		uint32_t expected = static_cast< uint32_t >( NOTIFY_ONE);
		detail::atomic_compare_exchange_strong(
				& cmd_, & expected,
				static_cast< uint32_t >( SLEEPING) );
		if ( static_cast< uint32_t >( SLEEPING) == expected)
			continue;
		else if ( static_cast< uint32_t >( NOTIFY_ONE) == expected)
		{
			unlock_enter_mtx = true;
			detail::atomic_fetch_sub( & waiters_, 1);
			break;
		}
		else
		{
			unlock_enter_mtx = 1 == detail::atomic_fetch_sub( & waiters_, 1);
			if ( unlock_enter_mtx)
			{
				expected = static_cast< uint32_t >( NOTIFY_ALL);
				detail::atomic_compare_exchange_strong(
						& cmd_, & expected,
						static_cast< uint32_t >( SLEEPING) );
			}
			break;
		}
	}

	if ( unlock_enter_mtx)
		enter_mtx_.unlock();

	mtx.lock();
}

bool
spin_condition::wait_( spin_mutex & mtx, system_time const& abs_time)
{
	if ( get_system_time() >= abs_time) return false;

	{
		spin_mutex::scoped_lock lk( enter_mtx_, abs_time);
		BOOST_ASSERT( lk);
		detail::atomic_fetch_add( & waiters_, 1);
		mtx.unlock();
	}

	bool timed_out = false, unlock_enter_mtx = false;
	for (;;)
	{
		while ( static_cast< uint32_t >( SLEEPING) == detail::atomic_load( & cmd_) )
		{
			this_thread::interruption_point();
			if ( this_task::runs_in_pool() )
				this_task::block();
			else
				this_thread::yield();	
			this_thread::interruption_point();

			if ( get_system_time() >= abs_time)
			{
				timed_out = enter_mtx_.try_lock();
				if ( ! timed_out)
					continue;
				break;
			}
		}

		if ( timed_out)
		{
			detail::atomic_fetch_sub( & waiters_, 1);
			unlock_enter_mtx = true;
			break;
		}
		else
		{
			spin_mutex::scoped_lock lk( check_mtx_);
			BOOST_ASSERT( lk);

			uint32_t expected = static_cast< uint32_t >( NOTIFY_ONE);
			detail::atomic_compare_exchange_strong(
					& cmd_, & expected,
					static_cast< uint32_t >( SLEEPING) );
			if ( static_cast< uint32_t >( SLEEPING) == expected)
				continue;
			else if ( static_cast< uint32_t >( NOTIFY_ONE) == expected)
			{
				unlock_enter_mtx = true;
				detail::atomic_fetch_sub( & waiters_, 1);
				break;
			}
			else
			{
				unlock_enter_mtx = 1 == detail::atomic_fetch_sub( & waiters_, 1);
				if ( unlock_enter_mtx)
				{
					expected = static_cast< uint32_t >( NOTIFY_ALL);
					detail::atomic_compare_exchange_strong(
							& cmd_, & expected,
							static_cast< uint32_t >( SLEEPING) );
				}
				break;
			}
		}
	}

	if ( unlock_enter_mtx)
		enter_mtx_.unlock();

	mtx.lock();

	return ! timed_out;
}

spin_condition::spin_condition()
:
cmd_( static_cast< uint32_t >( SLEEPING) ),
waiters_( 0),
enter_mtx_(),
check_mtx_()
{}

void
spin_condition::notify_one()
{ notify_( static_cast< uint32_t >( NOTIFY_ONE) ); }

void
spin_condition::notify_all()
{ notify_( static_cast< uint32_t >( NOTIFY_ALL) ); }

}}
