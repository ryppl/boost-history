
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASKS_BOUNDED_ONELOCK_FIFO_H
#define BOOST_TASKS_BOUNDED_ONELOCK_FIFO_H

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
#include <boost/task/watermark.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace tasks {

class bounded_onelock_fifo
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
	condition			not_full_cond_;
	std::size_t			hwm_;
	std::size_t			lwm_;

	bool active_() const
	{ return 0 == state_; }

	void deactivate_()
	{ detail::atomic_fetch_add( & state_, 1); }

	bool empty_() const
	{ return queue_.empty(); }

	bool full_() const
	{ return size_() >= hwm_; }

	std::size_t size_() const
	{ return queue_.size(); }

	void upper_bound_( std::size_t hwm)
	{
		if ( lwm_ > hwm )
			throw invalid_watermark("low watermark must be less than or equal to high watermark");
		std::size_t tmp( hwm_);
		hwm_ = hwm;
		if ( hwm_ > tmp) not_full_cond_.notify_one();
	}

	void lower_bound_( std::size_t lwm)
	{
		if ( lwm > hwm_ )
			throw invalid_watermark("low watermark must be less than or equal to high watermark");
		std::size_t tmp( lwm_);
		lwm_ = lwm;
		if ( lwm_ > tmp) not_full_cond_.notify_one();
	}

	void put_(
		value_type const& va,
		unique_lock< shared_mutex > & lk)
	{
		if ( full_() )
		{
			not_full_cond_.wait(
				lk,
				bind(
					& bounded_onelock_fifo::producers_activate_,
					this) );
		}
		if ( ! active_() )
			throw task_rejected("queue is not active");
		queue_.push_back( va);
		not_empty_cond_.notify_one();
	}

	template< typename Duration >
	void put_(
		value_type const& va,
		Duration const& rel_time,
		unique_lock< shared_mutex > & lk)
	{
		if ( full_() )
		{
			if ( ! not_full_cond_.timed_wait(
				lk,
				rel_time,
				bind(
					& bounded_onelock_fifo::producers_activate_,
					this) ) )
				throw task_rejected("timed out");
		}
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
						& bounded_onelock_fifo::consumers_activate_,
						this) );
			}
			catch ( thread_interrupted const&)
			{ return false; }
		}
		if ( ! active_() && empty_() )
			return false;
		va.swap( queue_.front() );
		queue_.pop_front();
		if ( size_() <= lwm_)
		{
			if ( lwm_ == hwm_)
				not_full_cond_.notify_one();
			else
				// more than one producer could be waiting
				// for submiting an action object
				not_full_cond_.notify_all();
		}
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
						& bounded_onelock_fifo::consumers_activate_,
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
		if ( size_() <= lwm_)
		{
			if ( lwm_ == hwm_)
				not_full_cond_.notify_one();
			else
				// more than one producer could be waiting
				// in order to submit an task
				not_full_cond_.notify_all();
		}
		return ! va.empty();
	}

	bool try_take_( value_type & va)
	{
		if ( empty_() )
			return false;
		va.swap( queue_.front() );
		queue_.pop_front();
		bool valid = ! va.empty();
		if ( valid && size_() <= lwm_)
		{
			if ( lwm_ == hwm_)
				not_full_cond_.notify_one();
			else
				// more than one producer could be waiting
				// in order to submit an task
				not_full_cond_.notify_all();
		}
		return valid;
	}

	bool producers_activate_() const
	{ return ! active_() || ! full_(); }

	bool consumers_activate_() const
	{ return ! active_() || ! empty_(); }

public:
	bounded_onelock_fifo(
			high_watermark const& hwm,
			low_watermark const& lwm) :
		state_( 0),
		queue_(),
		mtx_(),
		not_empty_cond_(),
		not_full_cond_(),
		hwm_( hwm),
		lwm_( lwm)
	{
		if ( lwm_ > hwm_ )
			throw invalid_watermark("low watermark must be less than or equal to high watermark");
	}

	void deactivate()
	{ deactivate_(); }

	bool empty()
	{
		shared_lock< shared_mutex > lk( mtx_);
		return empty_();
	}

	std::size_t upper_bound()
	{
		shared_lock< shared_mutex > lk( mtx_);
		return hwm_;
	}

	void upper_bound( std::size_t hwm)
	{
		unique_lock< shared_mutex > lk( mtx_);
		upper_bound_( hwm);
	}

	std::size_t lower_bound()
	{
		shared_lock< shared_mutex > lk( mtx_);
		return lwm_;
	}

	void lower_bound( std::size_t lwm)
	{
		unique_lock< shared_mutex > lk( mtx_);
		lower_bound_( lwm);
	}

	void put( value_type const& va)
	{
		unique_lock< shared_mutex > lk( mtx_);
		put_( va, lk);
	}

	template< typename Duration >
	void put(
		value_type const& va,
		Duration const& rel_time)
	{
		unique_lock< shared_mutex > lk( mtx_);
		put_( va, rel_time, lk);
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

#endif // BOOST_TASKS_BOUNDED_ONELOCK_FIFO_H
