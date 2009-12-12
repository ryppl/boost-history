
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
//  based on boost::interprocess::sync::interprocess_condition

#ifndef BOOST_FIBERS_CONDITION_H
#define BOOST_FIBERS_CONDITION_H

#include <boost/assert.hpp>
#include <boost/cstdint.hpp>
#include <boost/utility.hpp>
#include <boost/thread/locks.hpp>

#include <boost/fiber/detail/atomic.hpp>
#include <boost/fiber/exceptions.hpp>
#include <boost/fiber/mutex.hpp>
#include <boost/fiber/object/id.hpp>
#include <boost/fiber/scheduler.hpp>
#include <boost/fiber/strategy.hpp>

namespace boost {
namespace fibers {

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
	object::id			id_;
	strategy::ptr_t		strategy_;

public:
	template< typename Strategy >
	condition( scheduler< Strategy > & sched) :
		cmd_( static_cast< uint32_t >( SLEEPING) ),
		waiters_( 0),
		enter_mtx_( sched),
		check_mtx_( sched),
		id_( * this),
		strategy_( sched.strategy_)
	{ strategy_->register_object( id_); }

	~condition();

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
				strategy_->wait_for_object( id_);

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

}}

#endif // BOOST_FIBERS_CONDITION_H
