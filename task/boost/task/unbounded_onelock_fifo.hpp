
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASKS_UNBOUNDED_ONELOCK_FIFO_H
#define BOOST_TASKS_UNBOUNDED_ONELOCK_FIFO_H

#include <cstddef>
#include <list>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <boost/task/callable.hpp>
#include <boost/task/detail/atomic.hpp>
#include <boost/task/detail/meta.hpp>
#include <boost/task/exceptions.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace tasks {

class unbounded_onelock_fifo
{
public:
	typedef detail::has_no_attribute	attribute_tag_type;
	typedef callable					value_type;

private:
	typedef std::list< value_type >		queue_type;

	volatile uint32_t	state_;
	queue_type			queue_;
	shared_mutex		mtx_;
	condition			not_empty_cond_;

	bool active_() const
	{ return 0 == state_; }

	void deactivate_()
	{ detail::atomic_fetch_add( & state_, 1); }

	bool empty_() const
	{ return queue_.empty(); }

	void put_( value_type const& va)
	{
		if ( ! active_() )
			throw task_rejected("queue is not active");
		queue_.push_back( va);
		not_empty_cond_.notify_one();
	}

	bool take_(
		value_type & va,
		unique_lock< shared_mutex > & lk)
	{
		bool empty = empty_();
		if ( ! active_() && empty)
			return false;
		if ( empty)
		{
			try
			{
				not_empty_cond_.wait(
					lk,
					bind(
						& unbounded_onelock_fifo::consumers_activate_,
						this) );
			}
			catch ( thread_interrupted const&)
			{ return false; }
		}
		if ( ! active_() && empty_() )
			return false;
		va.swap( queue_.front() );
		queue_.pop_front();
		return ! va.empty();
	}

	template< typename Duration >
	bool take_(
		value_type & va,
		Duration const& rel_time,
		unique_lock< shared_mutex > & lk)
	{
		bool empty = empty_();
		if ( ! active_() && empty)
			return false;
		if ( empty)
		{
			try
			{
				if ( ! not_empty_cond_.timed_wait(
					lk,
					rel_time,
					bind(
						& unbounded_onelock_fifo::consumers_activate_,
						this) ) )
					return false;
			}
			catch ( thread_interrupted const&)
			{ return false; }
		}
		if ( ! active_() && empty_() )
			return false;
		va.swap( queue_.front() );
		queue_.pop_front();
		return ! va.empty();
	}

	bool try_take_( value_type & va)
	{
		if ( empty_() )
			return false;
		va.swap( queue_.front() );
		queue_.pop_front();
		return ! va.empty();
	}

	bool consumers_activate_() const
	{ return ! active_() || ! empty_(); }

public:
	unbounded_onelock_fifo() :
		state_( 0),
		queue_(),
		mtx_(),
		not_empty_cond_()
	{}

	void deactivate()
	{ deactivate_(); }

	bool empty()
	{
		shared_lock< shared_mutex > lk( mtx_);
		return empty_();
	}

	void put( value_type const& va)
	{
		unique_lock< shared_mutex > lk( mtx_);
		put_( va);
	}

	bool take( value_type & va)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return take_( va, lk);
	}

	template< typename Duration >
	bool take(
		value_type & va,
		Duration const& rel_time)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return take_( va, rel_time, lk);
	}

	bool try_take( value_type & va)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return try_take_( va);
	}
};

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASKS_UNBOUNDED_ONELOCK_FIFO_H
