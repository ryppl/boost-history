
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_UNBOUNDED_CHANNEL_H
#define BOOST_TASK_UNBOUNDED_CHANNEL_H

#include <cstddef>
#include <vector>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <boost/task/detail/pool_callable.hpp>
#include <boost/task/exceptions.hpp>

namespace boost { namespace task
{
template< typename SchedulingPolicy >
class unbounded_channel
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

	const std::vector< detail::pool_callable > drain_()
	{
		BOOST_ASSERT( deactive_now_() );
		std::vector< detail::pool_callable > unprocessed;
		unprocessed.reserve( queue_.size() );
		BOOST_FOREACH( detail::pool_callable ca, queue_)
		{ unprocessed.push_back( ca); }
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
		if ( ! active_() )
			throw task_rejected("channel is not active");
		queue_.push( itm);
		not_empty_cond_.notify_one();
	}

	bool take_(
		detail::pool_callable & ca,
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
		catch ( thread_interrupted const&)
		{ return false; }
		if ( deactive_now_() || ( deactive_() && empty_() ) )
			return false;
		ca = queue_.pop();
		return ! ca.empty();
	}

	template< typename Duration >
	bool take_(
		detail::pool_callable & ca,
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
		catch ( thread_interrupted const&)
		{ return false; }
		if ( deactive_now_() || ( deactive_() && empty_() ) )
			return false;
		ca = queue_.pop();
		return ! ca.empty();
	}

	bool try_take_( detail::pool_callable & ca)
	{
		if ( deactive_now_() || empty_() )
			return false;
		ca = queue_.pop();
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

	const std::vector< detail::pool_callable > drain()
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
		item & itm,
		Duration const&)
	{
		unique_lock< shared_mutex > lk( mtx_);
		put_( itm, lk);
	}

	bool take( detail::pool_callable & ca)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return take_( ca, lk);
	}

	template< typename Duration >
	bool take(
		detail::pool_callable & ca,
		Duration const& rel_time)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return take_( ca, rel_time, lk);
	}

	bool try_take( detail::pool_callable & ca)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return try_take_( ca);
	}
};
} }

#endif // BOOST_TASK_UNBOUNDED_CHANNEL_H
