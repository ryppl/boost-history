//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_UNBOUNDED_CHANNEL_H
#define BOOST_TP_UNBOUNDED_CHANNEL_H

#include <cstddef>
#include <list>
#include <utility>
#include <vector>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/ref.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <boost/tp/detail/interrupter.hpp>
#include <boost/tp/exceptions.hpp>
#include <boost/tp/fifo.hpp>

namespace boost { namespace tp
{
template<
	typename QueueingPolicy = fifo
>
class unbounded_channel
{
private:
	typedef function< void() >				callable;
	typedef QueueingPolicy					queueing_policy;
	typedef typename queueing_policy::template impl<
		callable
	>										queue;

public:
	typedef typename queue::item		item;
	typedef typename queue::iterator	iterator;
	typedef queueing_policy				queue_type;

private:
	bool			active_;
	queue			queue_;
	shared_mutex	mtx_;
	condition		not_empty_cond_;

	void activate_()
	{ active_ = true; }

	void clear_()
	{
		BOOST_ASSERT( ! active_);
		queue_.clear();
		BOOST_ASSERT( queue_.empty() );
	}

	void deactivate_()
	{
		if ( active_)
		{
			active_ = false;
			not_empty_cond_.notify_all();
		}

		BOOST_ASSERT( ! active_);
	}

	const std::vector< callable > drain_()
	{
		BOOST_ASSERT( ! active_);
		std::vector< callable > unprocessed;
		unprocessed.reserve( queue_.size() );
		BOOST_FOREACH( item itm, queue_)
		{ unprocessed.push_back( itm.ca() ); }
		clear_();
		return unprocessed;
	}

	bool empty_() const
	{ return queue_.empty(); }

	bool erase_(
		iterator & i,
		future< void > & f)
	{ return queue_.erase( i, f); }

	std::size_t size_() const
	{ return queue_.size(); }

	iterator put_(
		item const& itm,
		unique_lock< shared_mutex > & lk)
	{
		if ( ! active_)
			throw task_rejected("channel is not active");
		iterator i( queue_.push( itm) );
		not_empty_cond_.notify_one();
		return i;
	}

	bool take_(
		callable & ca,
		detail::interrupter & intr,
		unique_lock< shared_mutex > & lk)
	{
		if ( ! active_ && empty_() )
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
		if ( ! active_ && empty_() )
			return false;
		item itm( queue_.pop() );
		ca = itm.ca();
		intr = itm.intr();
		return ! ca.empty();
	}

	template< typename Tm >
	bool take_(
		callable & ca,
		detail::interrupter & intr,
		Tm const& tm,
		unique_lock< shared_mutex > & lk)
	{
		if ( ! active_ && empty_() )
			return false;
		try
		{
			if ( ! not_empty_cond_.timed_wait(
				lk,
				tm,
				bind(
					& unbounded_channel::consumers_activate_,
					this) ) )
				return false;
		}
		catch ( thread_interrupted const& e)
		{ return false; }
		if ( ! active_ && empty_() )
			return false;
		item itm( queue_.pop() );
		ca = itm.ca();
		intr = itm.intr();
		return ! ca.empty();
	}

	bool consumers_activate_() const
	{ return ! active_ || ! empty_(); }

public:
	unbounded_channel()
	:
	active_( false),
	queue_(),
	mtx_(),
	not_empty_cond_()
	{}

	bool active()
	{
		shared_lock< shared_mutex > lk( mtx_);
		return active_;
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

	const std::vector< callable > drain()
	{
		unique_lock< shared_mutex > lk( mtx_);
		return drain_();
	}

	bool empty()
	{ 
		shared_lock< shared_mutex > lk( mtx_);
		return empty_();
	}

	bool erase(
		iterator & i,
		future< void > f)
	{ 
		shared_lock< shared_mutex > lk( mtx_);
		return erase_( i, f);
	}

	bool full()
	{ return false; }

	std::size_t size()
	{ 
		shared_lock< shared_mutex > lk( mtx_);
		return size_();
	}

	iterator put( item const& itm)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return put_( itm, lk);
	}

	template< typename Tm >
	iterator put(
		item const& itm,
		Tm const&)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return put_( itm, lk);
	}

	bool take(
		callable & ca,
		detail::interrupter & intr)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return take_( ca, intr, lk);
	}

	template< typename Tm >
	bool take(
		callable & ca,
		detail::interrupter & intr,
		Tm const& tm)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return take_( ca, intr, tm, lk);
	}
};
} }

#endif // BOOST_TP_UNBOUNDED_CHANNEL_H
