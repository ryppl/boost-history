
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
//  based on boost::interprocess::sync::interprocess_condition

#ifndef BOOST_FIBERS_SPIN_CONDITION_H
#define BOOST_FIBERS_SPIN_CONDITION_H

#include <boost/assert.hpp>
#include <boost/cstdint.hpp>
#include <boost/thread/locks.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/detail/atomic.hpp>
#include <boost/fiber/exceptions.hpp>
#include <boost/fiber/spin/mutex.hpp>
#include <boost/fiber/utility.hpp>

namespace boost {
namespace fibers {
namespace spin {

class condition : private noncopyable
{
private:
	enum command_t
	{
		SLEEPING = 0,
		NOTIFY_ONE,
		NOTIFY_ALL
	};

	volatile uint32_t	cmd_;
	volatile uint32_t	waiters_;
	mutex				enter_mtx_;
	mutex				check_mtx_;

	void notify_( uint32_t);

public:
	condition();

	void notify_one();

	void notify_all();

	void wait( unique_lock< mutex > & lk)
	{
		if ( ! lk)
			throw lock_error();
		wait( * lk.mutex() );
	}

	template< typename Pred >
	void wait( unique_lock< mutex > & lk, Pred pred)
	{
		if ( ! lk)
			throw lock_error();

		while ( ! pred() )
			wait( * lk.mutex() );
	}

	template< typename LockType >
	void wait( LockType & lt)
	{
		{
			mutex::scoped_lock lk( enter_mtx_);
			BOOST_ASSERT( lk);
			detail::atomic_fetch_add( & waiters_, 1);
			lt.unlock();
		}

		bool unlock_enter_mtx = false;
		for (;;)
		{
			while ( static_cast< uint32_t >( SLEEPING) == detail::atomic_load( & cmd_) )
				this_fiber::yield();	

			mutex::scoped_lock lk( check_mtx_);
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

		lt.lock();
	}

	template<
		typename LockType,
		typename Pred
	>
	void wait( LockType & lt, Pred pred)
	{
		while ( ! pred() )
			wait( lt);
	}
};

}}}

#endif // BOOST_FIBERS_SPIN_CONDITION_H
