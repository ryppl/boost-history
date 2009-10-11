
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/spin_count_down_event.hpp"

#include <boost/thread.hpp>

#include <boost/task/detail/atomic.hpp>
#include <boost/task/spin_mutex.hpp>
#include <boost/task/spin_lock.hpp>
#include <boost/task/utility.hpp>

namespace boost {
namespace task {

spin_count_down_event::spin_count_down_event( uint32_t intial)
:
intial_( intial),
current_( intial_)
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
	enter_mtx_.lock();

	detail::atomic_fetch_sub( & current_, 1);
}

void
spin_count_down_event::wait()
{
	{
		spin_lock< spin_mutex > lk( enter_mtx_);
		if ( ! lk) return;
		detail::atomic_fetch_add( & waiters_, 1);
	}

	bool unlock_enter_mtx = false;
	for (;;)
	{
		while ( SLEEPING == detail::atomic_load( & cmd_) )
		{
			if ( this_task::runs_in_pool() )
				this_task::block();
			else
				this_thread::yield();	
		}

		spin_lock< spin_mutex > lk( check_mtx_);
		if ( ! lk)
		{
			unlock_enter_mtx = true;
			break;
		}

		uint32_t expected = static_cast< uint32_t >( NOTIFY_ONE);
		detail::atomic_compare_exchange_strong(
				static_cast< uint32_t volatile* >( & cmd_), & expected, SLEEPING);
		if ( SLEEPING == expected)
			continue;
		else if ( NOTIFY_ONE == expected)
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
						static_cast< uint32_t volatile* >( & cmd_), & expected, SLEEPING);
			}
			break;
		}
	}

	if ( unlock_enter_mtx)
		enter_mtx_.unlock();
}

bool
spin_count_down_event::wait( system_time const& abs_time)
{
	if ( get_system_time() >= abs_time) return false;

	{
		spin_lock< spin_mutex > lk( enter_mtx_, abs_time);
		if ( ! lk) return false;
		detail::atomic_fetch_add( & waiters_, 1);
	}

	bool timed_out = false, unlock_enter_mtx = false;
	for (;;)
	{
		while ( SLEEPING == detail::atomic_load( & cmd_) )
		{
			if ( this_task::runs_in_pool() )
				this_task::block();
			else
				this_thread::yield();	

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
			spin_lock< spin_mutex > lk( check_mtx_);
			if ( ! lk)
			{
				timed_out = true;
				unlock_enter_mtx = true;
				break;
			}

			uint32_t expected = static_cast< uint32_t >( NOTIFY_ONE);
			detail::atomic_compare_exchange_strong(
					static_cast< uint32_t volatile* >( & cmd_), & expected, SLEEPING);
			if ( SLEEPING == expected)
				continue;
			else if ( NOTIFY_ONE == expected)
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
							static_cast< uint32_t volatile* >( & cmd_), & expected, SLEEPING);
				}
				break;
			}
		}
	}

	if ( unlock_enter_mtx)
		enter_mtx_.unlock();

	return ! timed_out;
}

}}
