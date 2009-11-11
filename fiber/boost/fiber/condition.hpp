
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
//  based on boost::interprocess::sync::interprocess_condition

#ifndef BOOST_FIBERS_CONDITION_H
#define BOOST_FIBERS_CONDITION_H

#include <boost/cstdint.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/exceptions.hpp>
#include <boost/fiber/mutex.hpp>

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

	void wait_( mutex &);
	bool wait_( mutex &, system_time const&);
	void notify_( uint32_t);

public:
	condition();

	void notify_one();

	void notify_all();

	template< typename Lock >
	void wait( Lock & lk)
	{
		if ( ! lk)
			throw lock_error();
		wait_( * lk.mutex() );
	}

	template<
		typename Lock,
		typename Pred
	>
	void wait( Lock & lk, Pred pred)
	{
		if ( ! lk)
			throw lock_error();

		while ( ! pred() )
			wait_( * lk.mutex() );
	}

	template< typename Lock >
	bool timed_wait( Lock & lk, system_time const& abs_time)
	{
		if ( abs_time.is_infinity() )
		{
			wait( lk);
			return true;
		}

		if ( ! lk)
			throw lock_error();
		return wait_( * lk.mutex(), abs_time);
	}

	template<
		typename Lock,
		typename Pred
	>
	bool timed_wait( Lock & lk, system_time const& abs_time, Pred pred)
	{
		if ( abs_time.is_infinity() )
		{
			wait( lk, pred);
			return true;
		}

		if ( ! lk)
			throw lock_error();

		while ( ! pred() )
			if ( ! wait_( * lk.mutex(), abs_time) )
				return pred();
		return true;
	}

	template<
		typename Lock,
		typename TimeDuration
	>
	bool timed_wait( Lock & lk, TimeDuration const& rel_time)
	{ return timed_wait( lk, get_system_time() + rel_time); }

	template<
		typename Lock,
		typename TimeDuration,
		typename Pred
	>
	bool timed_wait( Lock & lk, TimeDuration const& rel_time, Pred pred)
	{ return timed_wait( lk, get_system_time() + rel_time, pred); }
};

}}

#endif // BOOST_FIBERS_CONDITION_H
