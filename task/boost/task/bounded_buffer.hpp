
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_BOUNDED_BUFFER_H
#define BOOST_TASK_BOUNDED_BUFFER_H

#include <cstddef>
#include <deque>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include <boost/task/detail/atomic.hpp>
#include <boost/task/exceptions.hpp>
#include <boost/task/semaphore.hpp>
#include <boost/task/spin_condition.hpp>
#include <boost/task/spin_unique_lock.hpp>
#include <boost/task/spin_mutex.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{
template< typename T >
class bounded_buffer
{
private:
	typedef T	value_type;

	class base
	{
	private:
		volatile uint32_t			state_;
		std::deque< value_type >	queue_;
		spin_mutex					mtx_;
		spin_condition				not_empty_cond_;
		spin_condition				not_full_cond_;
		std::size_t					hwm_;
		std::size_t					lwm_;

		base( base &);
		base & operator=( base const&);

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
			spin_unique_lock< spin_mutex > & lk)
		{
			if ( full_() )
			{
				not_full_cond_.wait(
					lk,
					bind(
						& base::producers_activate_,
						this) );
			}
			if ( ! active_() )
				throw task_rejected("queue is not active");
			queue_.push_back( va);
			not_empty_cond_.notify_one();
		}

		template< typename TimeDuration >
		void put_(
			value_type const& va,
			TimeDuration const& rel_time,
			spin_unique_lock< spin_mutex > & lk)
		{
			if ( full_() )
			{
				if ( ! not_full_cond_.timed_wait(
					lk,
					rel_time,
					bind(
						& base::producers_activate_,
						this) ) )
					throw task_rejected("timed out");
			}
			if ( ! active_() )
				throw task_rejected("queue is not active");
			queue_.push_back( va);
			not_empty_cond_.notify_one();
		}

		bool take_(
			optional< value_type > & va,
			spin_unique_lock< spin_mutex > & lk)
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
							& base::consumers_activate_,
							this) );
				}
				catch ( thread_interrupted const&)
				{ return false; }
			}
			if ( ! active_() && empty_() )
				return false;
			va = queue_.front();
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
			return va;
		}

		template< typename TimeDuration >
		bool take_(
			optional< value_type > & va,
			TimeDuration const& rel_time,
			spin_unique_lock< spin_mutex > & lk)
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
							& base::consumers_activate_,
							this) ) )
						return false;
				}
				catch ( thread_interrupted const&)
				{ return false; }
			}
			if ( ! active_() && empty_() )
				return false;
			va = queue_.front();
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
			return va;
		}

		bool try_take_( optional< value_type > & va)
		{
			if ( empty_() )
				return false;
			va = queue_.front();
			queue_.pop_front();
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

		bool producers_activate_() const
		{ return ! active_() || ! full_(); }

		bool consumers_activate_() const
		{ return ! active_() || ! empty_(); }

	public:
		base(
			high_watermark const& hwm,
			low_watermark const& lwm)
		:
		state_( 0),
		queue_(),
		mtx_(),
		not_empty_cond_(),
		not_full_cond_(),
		hwm_( hwm),
		lwm_( lwm)
		{}

		void deactivate()
		{ deactivate_(); }

		bool empty()
		{
			spin_unique_lock< spin_mutex > lk( mtx_);
			return empty_();
		}

		std::size_t upper_bound()
		{
			spin_unique_lock< spin_mutex > lk( mtx_);
			return hwm_;
		}
		
		void upper_bound( std::size_t hwm)
		{
			spin_unique_lock< spin_mutex > lk( mtx_);
			upper_bound_( hwm);
		}
		
		std::size_t lower_bound()
		{
			spin_unique_lock< spin_mutex > lk( mtx_);
			return lwm_;
		}
		
		void lower_bound( std::size_t lwm)
		{
			spin_unique_lock< spin_mutex > lk( mtx_);
			lower_bound_( lwm);
		}

		void put( value_type const& va)
		{
			spin_unique_lock< spin_mutex > lk( mtx_);
			put_( va);
		}

		template< typename TimeDuration >
		void put(
			value_type const& va,
			TimeDuration const& rel_time)
		{
			spin_unique_lock< spin_mutex > lk( mtx_);
			put_( va, rel_time, lk);
		}

		bool take( optional< value_type > & va)
		{
			spin_unique_lock< spin_mutex > lk( mtx_);
			return take_( va, lk);
		}

		template< typename TimeDuration >
		bool take(
			optional< value_type > & va,
			TimeDuration const& rel_time)
		{
			spin_unique_lock< spin_mutex > lk( mtx_);
			return take_( va, rel_time, lk);
		}

		bool try_take( optional< value_type > & va)
		{
			spin_unique_lock< spin_mutex > lk( mtx_);
			return try_take_( va);
		}
	};

	shared_ptr< base >		impl_;

public:
	bounded_buffer()
	: impl_( new base)
	{}

	void deactivate()
	{ impl_->deactivate(); }

	bool empty()
	{ return impl_->empty(); }

	std::size_t upper_bound()
	{ return impl_->upper_bound(); }

	void upper_bound( std::size_t hwm)
	{ impl_->upper_bound( hwm); }

	std::size_t lower_bound()
	{ return impl_->lower_bound; }

	void lower_bound( std::size_t lwm)
	{ impl_->lower_bound( lwm); }

	void put( T const& t)
	{ impl_->put( t); }

	template< typename TimeDuration >
	void put(
		value_type const& va,
		TimeDuration const& rel_time)
	{ impl_->put( va, rel_time); }

	bool take( optional< T > & t)
	{ return impl_->take( t); }

	template< typename TimeDuration >
	bool take(
		optional< T > & t,
		TimeDuration const& rel_time)
	{ return impl_->take( t, rel_time); }

	bool try_take( optional< T > & t)
	{ return impl_->try_take_( t); }
};
}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_BOUNDED_BUFFER_H
