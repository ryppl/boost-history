
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_BOUNDED_FIFO_H
#define BOOST_FIBERS_BOUNDED_FIFO_H

#include <cstddef>
#include <stdexcept>

#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/condition.hpp>
#include <boost/fiber/detail/atomic.hpp>
#include <boost/fiber/exceptions.hpp>
#include <boost/fiber/mutex.hpp>
#include <boost/fiber/spin_condition.hpp>
#include <boost/fiber/spin_mutex.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fibers {

template< typename T, typename M >
class bounded_fifo;

template< typename T >
class bounded_fifo< T, spin_mutex > : private noncopyable
{
public:
	typedef optional< T >	value_type;

private:
	struct node
	{
		typedef intrusive_ptr< node >	ptr_t;

		uint32_t	use_count;
		value_type	va;
		ptr_t		next;

		node() :
			use_count( 0),
			va(),
			next()
		{}

		inline friend void intrusive_ptr_add_ref( node * p)
		{ ++p->use_count; }
		
		inline friend void intrusive_ptr_release( node * p)
		{ if ( --p->use_count == 0) delete p; }
	};

	volatile uint32_t		state_;
	volatile uint32_t		count_;
	typename node::ptr_t	head_;
	spin_mutex				head_mtx_;
	typename node::ptr_t	tail_;
	spin_mutex				tail_mtx_;
	spin_condition			not_empty_cond_;
	spin_condition			not_full_cond_;
	std::size_t				hwm_;
	std::size_t				lwm_;
	volatile uint32_t		use_count_;

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

	typename node::ptr_t get_tail_()
	{
		spin_mutex::scoped_lock lk( tail_mtx_);	
		typename node::ptr_t tmp = tail_;
		return tmp;
	}

	typename node::ptr_t pop_head_()
	{
		typename node::ptr_t old_head = head_;
		head_ = old_head->next;
		detail::atomic_fetch_sub( & count_, 1);
		return old_head;
	}

public:
	bounded_fifo(
			std::size_t const& hwm,
			std::size_t const& lwm) :
		state_( 0),
		count_( 0),
		head_( new node),
		head_mtx_(),
		tail_( head_),
		tail_mtx_(),
		not_empty_cond_(),
		not_full_cond_(),
		hwm_( hwm),
		lwm_( lwm),
		use_count_( 0)
	{
		if ( hwm_ < lwm_)
			throw invalid_watermark();
	}

	bounded_fifo( std::size_t const& wm) :
		state_( 0),
		count_( 0),
		head_( new node),
		head_mtx_(),
		tail_( head_),
		tail_mtx_(),
		not_empty_cond_(),
		not_full_cond_(),
		hwm_( wm),
		lwm_( wm),
		use_count_( 0)
	{}

	void upper_bound_( std::size_t hwm)
	{
		if ( hwm < lwm_)
			throw invalid_watermark();
		std::size_t tmp( hwm_);
		hwm_ = hwm;
		if ( hwm_ > tmp) not_full_cond_.notify_one();
	}

	std::size_t upper_bound()
	{ return hwm_; }

	void lower_bound_( std::size_t lwm)
	{
		if ( lwm > hwm_ )
			throw invalid_watermark();
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
		spin_mutex::scoped_lock lk( head_mtx_);
		return empty_();
	}

	void put( T const& t)
	{
		typename node::ptr_t new_node( new node);
		{
			spin_mutex::scoped_lock lk( tail_mtx_);

			if ( full_() )
			{
				while ( active_() && full_() )
					not_full_cond_.wait( lk);
			}
			if ( ! active_() )
				throw std::runtime_error("queue is not active");

			tail_->va = t;
			tail_->next = new_node;
			tail_ = new_node;
			detail::atomic_fetch_add( & count_, 1);
		}
		not_empty_cond_.notify_one();
	}

	bool take( value_type & va)
	{
		spin_mutex::scoped_lock lk( head_mtx_);
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
			catch ( fiber_interrupted const&)
			{ return false; }
		}
		if ( ! active_() && empty_() )
			return false;
		swap( va, head_->va);
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
		return va;
	}

	bool try_take( value_type & va)
	{
		spin_mutex::scoped_lock lk( head_mtx_);
		if ( empty_() )
			return false;
		swap( va, head_->va);
		pop_head_();
		bool valid = va;
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

	template< typename R >
    friend void intrusive_ptr_add_ref( bounded_fifo< R, spin_mutex > * p)
    { detail::atomic_fetch_add( & p->use_count_, 1); }

	template< typename R >
    friend void intrusive_ptr_release( bounded_fifo< R, spin_mutex > * p)
    { if ( detail::atomic_fetch_sub( & p->use_count_, 1) == 1) delete p; }
};

template< typename T >
class bounded_fifo< T, mutex > : private noncopyable
{
public:
	typedef optional< T >	value_type;

private:
	struct node
	{
		typedef intrusive_ptr< node >	ptr_t;

		uint32_t	use_count;
		value_type	va;
		ptr_t		next;

		node() :
			use_count( 0),
			va(),
			next()
		{}

		inline friend void intrusive_ptr_add_ref( node * p)
		{ ++p->use_count; }
		
		inline friend void intrusive_ptr_release( node * p)
		{ if ( --p->use_count == 0) delete p; }
	};

	volatile uint32_t		state_;
	volatile uint32_t		count_;
	typename node::ptr_t	head_;
	mutex				head_mtx_;
	typename node::ptr_t	tail_;
	mutex				tail_mtx_;
	condition			not_empty_cond_;
	condition			not_full_cond_;
	std::size_t				hwm_;
	std::size_t				lwm_;
	volatile uint32_t		use_count_;

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

	typename node::ptr_t get_tail_()
	{
		mutex::scoped_lock lk( tail_mtx_);	
		typename node::ptr_t tmp = tail_;
		return tmp;
	}

	typename node::ptr_t pop_head_()
	{
		typename node::ptr_t old_head = head_;
		head_ = old_head->next;
		detail::atomic_fetch_sub( & count_, 1);
		return old_head;
	}

public:
	template< typename Strategy >
	bounded_fifo(
			scheduler< Strategy > & sched,
			std::size_t const& hwm,
			std::size_t const& lwm) :
		state_( 0),
		count_( 0),
		head_( new node),
		head_mtx_( sched),
		tail_( head_),
		tail_mtx_( sched),
		not_empty_cond_( sched),
		not_full_cond_( sched),
		hwm_( hwm),
		lwm_( lwm),
		use_count_( 0)
	{
		if ( hwm_ < lwm_)
			throw invalid_watermark();
	}

	template< typename Strategy >
	bounded_fifo(
			scheduler< Strategy > & sched,
			std::size_t const& wm) :
		state_( 0),
		count_( 0),
		head_( new node),
		head_mtx_( sched),
		tail_( head_),
		tail_mtx_( sched),
		not_empty_cond_( sched),
		not_full_cond_( sched),
		hwm_( wm),
		lwm_( wm),
		use_count_( 0)
	{}

	void upper_bound_( std::size_t hwm)
	{
		if ( hwm < lwm_)
			throw invalid_watermark();
		std::size_t tmp( hwm_);
		hwm_ = hwm;
		if ( hwm_ > tmp) not_full_cond_.notify_one();
	}

	std::size_t upper_bound()
	{ return hwm_; }

	void lower_bound_( std::size_t lwm)
	{
		if ( lwm > hwm_ )
			throw invalid_watermark();
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
		mutex::scoped_lock lk( head_mtx_);
		return empty_();
	}

	void put( T const& t)
	{
		typename node::ptr_t new_node( new node);
		{
			mutex::scoped_lock lk( tail_mtx_);

			if ( full_() )
			{
				while ( active_() && full_() )
					not_full_cond_.wait( lk);
			}
			if ( ! active_() )
				throw std::runtime_error("queue is not active");

			tail_->va = t;
			tail_->next = new_node;
			tail_ = new_node;
			detail::atomic_fetch_add( & count_, 1);
		}
		not_empty_cond_.notify_one();
	}

	bool take( value_type & va)
	{
		mutex::scoped_lock lk( head_mtx_);
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
			catch ( fiber_interrupted const&)
			{ return false; }
		}
		if ( ! active_() && empty_() )
			return false;
		swap( va, head_->va);
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
		return va;
	}

	bool try_take( value_type & va)
	{
		mutex::scoped_lock lk( head_mtx_);
		if ( empty_() )
			return false;
		swap( va, head_->va);
		pop_head_();
		bool valid = va;
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

	template< typename R >
    friend void intrusive_ptr_add_ref( bounded_fifo< R, mutex > * p)
    { detail::atomic_fetch_add( & p->use_count_, 1); }

	template< typename R >
    friend void intrusive_ptr_release( bounded_fifo< R, mutex > * p)
    { if ( detail::atomic_fetch_sub( & p->use_count_, 1) == 1) delete p; }
};

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_BOUNDED_FIFO_H
