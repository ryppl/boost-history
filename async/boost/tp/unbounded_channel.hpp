//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_UNBOUNDED_CHANNEL_H
#define BOOST_TP_UNBOUNDED_CHANNEL_H

#include <cstddef>
#include <vector>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/ref.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <boost/tp/detail/callable.hpp>
#include <boost/tp/detail/interrupter.hpp>
#include <boost/tp/exceptions.hpp>

namespace boost { namespace tp
{
template< typename SchedulingPolicy >
class unbounded_channel
{
private:
	typedef SchedulingPolicy					scheduling_policy;
	typedef typename scheduling_policy::template impl<
		detail::callable
	>											queue;

public:
	typedef typename queue::item		item;
	typedef scheduling_policy			scheduler_type;

private:
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
		BOOST_FOREACH( item itm, queue_)
		{ unprocessed.push_back( itm.ca() ); }
		clear_();
		BOOST_ASSERT( empty_() );
		return unprocessed;
	}

	bool empty_() const
	{ return queue_.empty(); }

	std::size_t size_() const
	{ return queue_.size(); }

	void put_(
		item const& itm,
		unique_lock< shared_mutex > & lk)
	{
		queue_.push( itm);
		not_empty_cond_.notify_one();
	}

	bool take_(
		detail::callable & ca,
		detail::interrupter & intr,
		unique_lock< shared_mutex > & lk)
	{
		if ( deactive_now_() || ( deactive_() && empty_() ) )
			return false;
		try
		{
			not_empty_cond_.wait(
				lk,
				bind(
					& unbounded_channel::consumers_activate_,
					this) );
		}
		catch ( thread_interrupted const& e)
		{ return false; }
		if ( deactive_now_() || ( deactive_() && empty_() ) )
			return false;
		item itm( queue_.pop() );
		ca = itm.ca();
		intr = itm.intr();
		return ! ca.empty();
	}

	template< typename Duration >
	bool take_(
		detail::callable & ca,
		detail::interrupter & intr,
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
					& unbounded_channel::consumers_activate_,
					this) ) )
				return false;
		}
		catch ( thread_interrupted const& e)
		{ return false; }
		if ( deactive_now_() || ( deactive_() && empty_() ) )
			return false;
		item itm( queue_.pop() );
		ca = itm.ca();
		intr = itm.intr();
		return ! ca.empty();
	}

	bool try_take_(
		detail::callable & ca,
		detail::interrupter & intr)
	{
		if ( deactive_now_() || empty_() )
			return false;
		item itm( queue_.pop() );
		ca = itm.ca();
		intr = itm.intr();
		return ! ca.empty();
	}

	bool consumers_activate_() const
	{ return ! active_() || ! empty_(); }

public:
	unbounded_channel()
	:
	state_( channel_active),
	queue_(),
	mtx_(),
	not_empty_cond_()
	{}

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
	{ return false; }

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
		Duration const&)
	{
		unique_lock< shared_mutex > lk( mtx_);
		put_( itm, lk);
	}

	bool take(
		detail::callable & ca,
		detail::interrupter & intr)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return take_( ca, intr, lk);
	}

	template< typename Duration >
	bool take(
		detail::callable & ca,
		detail::interrupter & intr,
		Duration const& rel_time)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return take_( ca, intr, rel_time, lk);
	}

	bool try_take(
		detail::callable & ca,
		detail::interrupter & intr)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return try_take_( ca, intr);
	}
};
} }

#endif // BOOST_TP_UNBOUNDED_CHANNEL_H
