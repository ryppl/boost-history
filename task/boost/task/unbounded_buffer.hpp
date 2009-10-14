
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_UNBOUNDED_BUFFER_H
#define BOOST_TASK_UNBOUNDED_BUFFER_H

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
class unbounded_buffer
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

		base( base &);
		base & operator=( base const&);

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
			return va;
		}

		template< typename Duration >
		bool take_(
			optional< value_type > & va,
			Duration const& rel_time,
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
			return va;
		}

		bool try_take_( optional< value_type > & va)
		{
			if ( empty_() )
				return false;
			va = queue_.front();
			queue_.pop_front();
			return va;
		}

		bool consumers_activate_() const
		{ return ! active_() || ! empty_(); }

	public:
		base()
		:
		state_( 0),
		queue_(),
		mtx_(),
		not_empty_cond_()
		{}

		bool active()
		{ return active_(); }

		void deactivate()
		{ deactivate_(); }

		bool empty()
		{
			spin_unique_lock< spin_mutex > lk( mtx_);
			return empty_();
		}

		void put( value_type const& va)
		{
			spin_unique_lock< spin_mutex > lk( mtx_);
			put_( va);
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
	unbounded_buffer()
	: impl_( new base)
	{}

	bool active()
	{ return impl_->active(); }

	void deactivate()
	{ impl_->deactivate(); }

	bool empty()
	{ return impl_->empty(); }

	void put( T const& t)
	{ impl_->put( t); }

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

#endif // BOOST_TASK_UNBOUNDED_BUFFER_H
