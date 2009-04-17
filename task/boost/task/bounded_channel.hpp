
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_BOUNDED_CHANNEL_H
#define BOOST_TASK_BOUNDED_CHANNEL_H

#include <cstddef>
#include <vector>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <boost/task/detail/callable.hpp>
#include <boost/task/exceptions.hpp>
#include <boost/task/watermark.hpp>

namespace boost { namespace task
{
template< typename SchedulingPolicy >
class bounded_channel
{
public:
	typedef SchedulingPolicy					scheduler_type;
	typedef typename scheduler_type::impl::item	item;

private:
	typedef typename scheduler_type::impl	queue;

	enum channel_state
	{
		channel_active,
		channel_deactive,
		channel_deactive_now
	};

	channel_state	state_;
	queue			queue_;
	shared_mutex	mtx_;
	condition		not_empty_cond_;
	condition		not_full_cond_;
	std::size_t		hwm_;
	std::size_t		lwm_;

	bool active_() const
	{ return state_ == channel_active; }

	bool deactive_() const
	{ return state_ == channel_deactive; }

	bool deactive_now_() const
	{ return state_ == channel_deactive_now; }

	void activate_()
	{ state_ = channel_active; }

	void clear_()
	{
		BOOST_ASSERT( ! active_() );
		queue_.clear();
		BOOST_ASSERT( empty_() );
	}

	void deactivate_()
	{
		if ( active_() )
		{
			state_ = channel_deactive;
			not_empty_cond_.notify_all();
		}

		BOOST_ASSERT( deactive_() );
	}

	void deactivate_now_()
	{
		if ( active_() )
		{
			state_ = channel_deactive_now;
			not_empty_cond_.notify_all();
		}

		BOOST_ASSERT( deactive_now_() );
	}

	const std::vector< detail::callable > drain_()
	{
		BOOST_ASSERT( deactive_now_() );
		std::vector< detail::callable > unprocessed;
		unprocessed.reserve( queue_.size() );
		BOOST_FOREACH( detail::callable ca, queue_)
		{ unprocessed.push_back( ca); }
		clear_();
		BOOST_ASSERT( empty_() );
		return unprocessed;
	}

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
		item const& itm,
		unique_lock< shared_mutex > & lk)
	{
		not_full_cond_.wait(
			lk,
			bind(
				& bounded_channel::producers_activate_,
				this) );
		if ( ! active_() )
			throw task_rejected("channel is not active");
		queue_.push( itm);
		not_empty_cond_.notify_one();
	}

	template< typename Duration >
	void put_(
		item const& itm,
		Duration const& rel_time,
		unique_lock< shared_mutex > & lk)
	{
		if ( ! not_full_cond_.timed_wait(
			lk,
			rel_time,
			bind(
				& bounded_channel::producers_activate_,
				this) ) )
			throw task_rejected("timed out");
		if ( ! active_() )
			throw task_rejected("channel is not active");
		queue_.push( itm);
		not_empty_cond_.notify_one();
	}

	bool take_(
		detail::callable & ca,
		unique_lock< shared_mutex > & lk)
	{
		if ( deactive_now_() || ( deactive_() && empty_() ) )
			return false;
		try
		{
			not_empty_cond_.wait(
				lk,
				bind(
					& bounded_channel::consumers_activate_,
					this) );
		}
		catch ( thread_interrupted const&)
		{ return false; }
		if ( deactive_now_() || ( deactive_() && empty_() ) )
			return false;
		ca = queue_.pop();
		if ( size_() <= lwm_)
		{
			if ( lwm_ == hwm_)
				not_full_cond_.notify_one();
			else
				// more than one producer could be waiting
				// for submiting an action object
				not_full_cond_.notify_all();
		}
		return ! ca.empty();
	}

	template< typename Duration >
	bool take_(
		detail::callable & ca,
		Duration const& rel_time,
		unique_lock< shared_mutex > & lk)
	{
		if ( deactive_now_() || ( deactive_() && empty_() ) )
			return false;
		try
		{
			if ( ! not_empty_cond_.timed_wait(
				lk,
				rel_time,
				bind(
					& bounded_channel::consumers_activate_,
					this) ) )
				return false;
		}
		catch ( thread_interrupted const&)
		{ return false; }
		if ( deactive_now_() || ( deactive_() && empty_() ) )
			return false;
		ca = queue_.pop();
		if ( size_() <= lwm_)
		{
			if ( lwm_ == hwm_)
				not_full_cond_.notify_one();
			else
				// more than one producer could be waiting
				// in order to submit an task
				not_full_cond_.notify_all();
		}
		return ! ca.empty();
	}

	bool try_take_( detail::callable & ca)
	{
		if ( deactive_now_() || empty_() )
			return false;
		ca = queue_.pop();
		if ( size_() <= lwm_)
		{
			if ( lwm_ == hwm_)
				not_full_cond_.notify_one();
			else
				// more than one producer could be waiting
				// in order to submit an task
				not_full_cond_.notify_all();
		}
		return ! ca.empty();
	}

	bool producers_activate_() const
	{ return ! active_() || ! full_(); }

	bool consumers_activate_() const
	{ return ! active_() || ! empty_(); }

public:
	bounded_channel(
		high_watermark const& hwm,
		low_watermark const& lwm)
	:
	state_( channel_active),
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

	bool active()
	{
		shared_lock< shared_mutex > lk( mtx_);
		return active_();
	}

	void activate()
	{
		unique_lock< shared_mutex > lk( mtx_);
		activate_();
	}

	void clear()
	{
		unique_lock< shared_mutex > lk( mtx_);
		clear_();
	}

	bool deactive()
	{ return ! active(); }

	void deactivate()
	{
		unique_lock< shared_mutex > lk( mtx_);
		deactivate_();
	}

	void deactivate_now()
	{
		unique_lock< shared_mutex > lk( mtx_);
		deactivate_now_();
	}

	const std::vector< detail::callable > drain()
	{
		unique_lock< shared_mutex > lk( mtx_);
		return drain_();
	}

	bool empty()
	{ 
		shared_lock< shared_mutex > lk( mtx_);
		return empty_();
	}

	bool full()
	{
		shared_lock< shared_mutex > lk( mtx_);
		return full_();
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

	std::size_t size()
	{ 
		shared_lock< shared_mutex > lk( mtx_);
		return size_();
	}

	void put( item const& itm)
	{
		unique_lock< shared_mutex > lk( mtx_);
		put_( itm, lk);
	}

	template< typename Duration >
	void put(
		item const& itm,
		Duration const& rel_time)
	{
		unique_lock< shared_mutex > lk( mtx_);
		put_( itm, rel_time, lk);
	}

	bool take( detail::callable & ca)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return take_( ca, lk);
	}

	template< typename Duration >
	bool take(
		detail::callable & ca,
		Duration const& rel_time)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return take_( ca, rel_time, lk);
	}

	bool try_take( detail::callable & ca)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return try_take_( ca);
	}
};
} }

#endif // BOOST_TASK_BOUNDED_CHANNEL_H
