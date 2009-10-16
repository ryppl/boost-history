
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_UNBOUNDED_ONELOCK_SMART_QUEUE_H
#define BOOST_TASK_UNBOUNDED_ONELOCK_SMART_QUEUE_H

#include <algorithm>
#include <cstddef>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <boost/task/callable.hpp>
#include <boost/task/detail/atomic.hpp>
#include <boost/task/detail/meta.hpp>
#include <boost/task/detail/smart.hpp>
#include <boost/task/exceptions.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace task {

template<
	typename Attr,
	typename Comp,
	typename Enq = detail::replace_oldest,
	typename Deq = detail::take_oldest
>
class unbounded_onelock_smart_queue
{
public:
	typedef detail::has_attribute	attribute_tag_type;
	typedef Attr					attribute_type;

	struct value_type
	{
		callable		ca;
		attribute_type	attr;

		value_type(
				callable const& ca_,
				attribute_type const& attr_) :
			ca( ca_), attr( attr_)
		{ BOOST_ASSERT( ! ca.empty() ); }

		void swap( value_type & other)
		{
			ca.swap( other.ca);
			std::swap( attr, other.attr);
		}
	};

private:
	typedef multi_index::multi_index_container<
		value_type,
		multi_index::indexed_by<
			multi_index::ordered_non_unique<
				multi_index::member<
					value_type,
					Attr,
					& value_type::attr
				>,
				Comp
			>
		>
	>															queue_type;
	typedef typename queue_type::template nth_index< 0 >::type	queue_index;

	volatile uint32_t	state_;
	queue_type			queue_;
	queue_index	&		idx_;
	shared_mutex		mtx_;
	condition			not_empty_cond_;
	Enq					enq_op_;
	Deq					deq_op_;

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
		enq_op_( idx_, va);
		not_empty_cond_.notify_one();
	}

	bool take_(
		callable & ca,
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
						& unbounded_onelock_smart_queue::consumers_activate_,
						this) );
			}
			catch ( thread_interrupted const&)
			{ return false; }
		}
		if ( ! active_() && empty_() )
			return false;
		deq_op_( idx_, ca);
		return ! ca.empty();
	}

	template< typename Duration >
	bool take_(
		callable & ca,
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
						& unbounded_onelock_smart_queue::consumers_activate_,
						this) ) )
					return false;
			}
			catch ( thread_interrupted const&)
			{ return false; }
		}
		if ( ! active_() && empty_() )
			return false;
		deq_op_( idx_, ca);
		return ! ca.empty();
	}

	bool try_take_( callable & ca)
	{
		if ( empty_() )
			return false;
		deq_op_( idx_, ca);
		return ! ca.empty();
	}

	bool consumers_activate_() const
	{ return ! active_() || ! empty_(); }

public:
	unbounded_onelock_smart_queue() :
		state_( 0),
		queue_(),
		idx_( queue_.get< 0 >() ),
		mtx_(),
		not_empty_cond_(),
		enq_op_(),
		deq_op_()
	{}

	void deactivate()
	{ detail::atomic_fetch_add( & state_, 1); }

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

	bool take( callable & ca)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return take_( ca, lk);
	}

	template< typename Duration >
	bool take(
		callable & ca,
		Duration const& rel_time)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return take_( ca, rel_time, lk);
	}

	bool try_take( callable & ca)
	{
		unique_lock< shared_mutex > lk( mtx_);
		return try_take_( ca);
	}
};

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_UNBOUNDED_ONELOCK_SMART_QUEUE_H
