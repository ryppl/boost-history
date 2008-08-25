//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_BOUNDED_CHANNEL_H
#define BOOST_TP_BOUNDED_CHANNEL_H

#include <cstddef>
#include <list>
#include <utility>
#include <vector>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <boost/tp/detail/interrupter.hpp>
#include <boost/tp/exceptions.hpp>
#include <boost/tp/fifo.hpp>
#include <boost/tp/watermark.hpp>

namespace boost { namespace tp
{
template<
	typename QueueingPolicy = fifo
>
class bounded_channel
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

private:
	bool			active_;
	queue			queue_;
	shared_mutex	mtx_;
	condition		not_empty_cond_;
	condition		not_full_cond_;
	std::size_t		hwm_;
	std::size_t		lwm_;

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
			not_full_cond_.notify_all();
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

	iterator put_(
		item const& itm,
		unique_lock< shared_mutex > & lk)
	{
		if ( ! active_)
			throw task_rejected("channel is not active");
		not_full_cond_.wait(
			lk,
			bind(
				& bounded_channel::producers_activate_,
				this) );
		if ( ! active_)
			throw task_rejected("channel is not active");
		iterator i( queue_.push( itm) );
		not_empty_cond_.notify_one();
		return i;
	}

	template< typename Tm >
	iterator put_(
		item const& itm,
		Tm const& tm,
		unique_lock< shared_mutex > & lk)
	{
		if ( ! active_)
			throw task_rejected("channel is not active");
		if ( ! not_full_cond_.timed_wait(
			lk,
			tm,
			bind(
				& bounded_channel::producers_activate_,
				this) ) )
			throw task_rejected("timed out");
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
					& bounded_channel::consumers_activate_,
					this) );
		}
		catch ( thread_interrupted const& e)
		{ return false; }
		if ( ! active_ && empty_() )
			return false;
		item itm( queue_.pop() );
		ca = itm.ca();
		intr = itm.intr();
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
					& bounded_channel::consumers_activate_,
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

	bool producers_activate_() const
	{ return ! active_ || ! full_(); }

	bool consumers_activate_() const
	{ return ! active_ || ! empty_(); }

public:
	bounded_channel(
		high_watermark const& hwm,
		low_watermark const& lwm)
	:
	active_( false),
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
	{
		shared_lock< shared_mutex > lk( mtx_);
		return full_();
	}

	const std::size_t upper_bound()
	{
		shared_lock< shared_mutex > lk( mtx_);
		return hwm_;
	}

	void upper_bound( std::size_t hwm)
	{
		unique_lock< shared_mutex > lk( mtx_);
		upper_bound_( hwm);
	}

	const std::size_t lower_bound()
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

	iterator put( item const& itm)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return put_( itm, lk);
	}

	template< typename Tm >
	iterator put(
		item const& itm,
		Tm const& tm)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return put_( itm, tm, lk);
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

#endif // BOOST_TP_BOUNDED_CHANNEL_H
