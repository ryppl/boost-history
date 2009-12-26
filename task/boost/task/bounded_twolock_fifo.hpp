
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASKS_BOUNDED_TWOLOCK_FIFO_H
#define BOOST_TASKS_BOUNDED_TWOLOCK_FIFO_H

#include <cstddef>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/cstdint.hpp>
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

class bounded_twolock_fifo
{
public:
	typedef detail::has_no_attribute	attribute_tag_type;
	typedef callable					value_type;

private:
	struct node
	{
		typedef shared_ptr< node >	sptr_t;

		value_type	va;
		sptr_t		next;
	};

	volatile uint32_t	state_;
	volatile uint32_t	count_;
	node::sptr_t		head_;
	mutex				head_mtx_;
	node::sptr_t		tail_;
	mutex				tail_mtx_;
	condition			not_empty_cond_;
	condition			not_full_cond_;
	std::size_t			hwm_;
	std::size_t			lwm_;

	bool active_() const
	{ return 0 == state_; }

	void deactivate_()
	{ detail::atomic_fetch_add( & state_, 1); }

	uint32_t size_()
	{ return count_; }

	bool empty_()
	{ return head_ == get_tail_(); }

	bool full_()
	{ return size_() >= hwm_; }

	node::sptr_t get_tail_()
	{
		lock_guard< mutex > lk( tail_mtx_);	
		node::sptr_t tmp = tail_;
		return tmp;
	}

	node::sptr_t pop_head_()
	{
		node::sptr_t old_head = head_;
		head_ = old_head->next;
		detail::atomic_fetch_sub( & count_, 1);
		return old_head;
	}

public:
	bounded_twolock_fifo(
			high_watermark const& hwm,
			low_watermark const& lwm) :
		state_( 0),
		count_( 0),
		head_( new node),
		head_mtx_(),
		tail_( head_),
		tail_mtx_(),
		not_empty_cond_(),
		not_full_cond_(),
		hwm_( hwm),
		lwm_( lwm)
	{}

	void upper_bound_( std::size_t hwm)
	{
		if ( lwm_ > hwm )
			throw invalid_watermark("low watermark must be less than or equal to high watermark");
		std::size_t tmp( hwm_);
		hwm_ = hwm;
		if ( hwm_ > tmp) not_full_cond_.notify_one();
	}

	std::size_t upper_bound()
	{ return hwm_; }

	void lower_bound_( std::size_t lwm)
	{
		if ( lwm > hwm_ )
			throw invalid_watermark("low watermark must be less than or equal to high watermark");
		std::size_t tmp( lwm_);
		lwm_ = lwm;
		if ( lwm_ > tmp) not_full_cond_.notify_one();
	}

	std::size_t lower_bound()
	{ return lwm_; }

	void deactivate()
	{ deactivate_(); }

	bool empty()
	{
		unique_lock< mutex > lk( head_mtx_);
		return empty_();
	}

	void put( value_type const& va)
	{
		node::sptr_t new_node( new node);
		{
			unique_lock< mutex > lk( tail_mtx_);

			if ( full_() )
			{
				while ( active_() && full_() )
					not_full_cond_.wait( lk);
			}
			if ( ! active_() )
				throw task_rejected("queue is not active");

			tail_->va = va;
			tail_->next = new_node;
			tail_ = new_node;
			detail::atomic_fetch_add( & count_, 1);
		}
		not_empty_cond_.notify_one();
	}

	template< typename Duration >
	void put(
		value_type const& va,
		Duration const& rel_time)
	{
		node::sptr_t new_node( new node);
		{
			unique_lock< mutex > lk( tail_mtx_);

			if ( full_() )
			{
				while ( active_() && full_() )
					if ( ! not_full_cond_.wait( lk, rel_time) )
						throw task_rejected("timed out");
			}
			if ( ! active_() )
				throw task_rejected("queue is not active");

			tail_->va = va;
			tail_->next = new_node;
			tail_ = new_node;
			detail::atomic_fetch_add( & count_, 1);
		}
		not_empty_cond_.notify_one();
	}

	bool take( value_type & va)
	{
		unique_lock< mutex > lk( head_mtx_);
		bool empty = empty_();
		if ( ! active_() && empty)
			return false;
		if ( empty)
		{
			try
			{
				while ( active_() && empty_() )
					not_empty_cond_.wait( lk);
			}
			catch ( thread_interrupted const&)
			{ return false; }
		}
		if ( ! active_() && empty_() )
			return false;
		va.swap( head_->va);
		pop_head_();
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
	bool take(
		value_type & va,
		Duration const& rel_time)
	{
		unique_lock< mutex > lk( head_mtx_);
		bool empty = empty_();
		if ( ! active_() && empty)
			return false;
		if ( empty)
		{
			try
			{
				while ( active_() && empty_() )
					if ( ! not_empty_cond_.timed_wait( lk, rel_time) )
						return false;
			}
			catch ( thread_interrupted const&)
			{ return false; }
		}
		if ( ! active_() && empty_() )
			return false;
		va.swap( head_->va);
		pop_head_();
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

	bool try_take( value_type & va)
	{
		unique_lock< mutex > lk( head_mtx_);
		if ( empty_() )
			return false;
		va.swap( head_->va);
		pop_head_();
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
};

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASKS_BOUNDED_TWOLOCK_FIFO_H
