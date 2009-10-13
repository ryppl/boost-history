//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_DETAIL_WSQ_H
#define BOOST_TP_DETAIL_WSQ_H

#include <utility>

#include <boost/assert.hpp>
#include <boost/interprocess/detail/atomic.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/utility.hpp>

#include <boost/tp/detail/interrupter.hpp>

namespace boost { namespace tp {
namespace detail
{
template< typename T >
class wsq
: private noncopyable
{
private:
	class impl
	{
	private:
		const int			initial_size_;
		shared_array< T >	array_;
		int					capacity_;
		int					mask_;
		volatile uint32_t	head_idx_;
		volatile uint32_t	tail_idx_;
		recursive_mutex		mtx_;
	
	public:
		impl()
		:
		initial_size_( 32),
		array_( new T[ initial_size_]),
		capacity_( initial_size_),
		mask_( initial_size_ - 1),
		head_idx_( 0),
		tail_idx_( 0),
		mtx_()
		{}
	
		bool empty() const
		{ return head_idx_ >= tail_idx_; }
	
		std::size_t size() const
		{ return tail_idx_ - head_idx_; }
	
		void put( T const& t)
		{
			uint32_t tail( tail_idx_);
			if ( tail <= head_idx_ + mask_)
			{
				array_[tail & mask_] = t;
				tail_idx_ = tail + 1;
			}
			else
			{
				unique_lock< recursive_mutex > lk( mtx_);
				uint32_t head( head_idx_);
				int count( size() );
	
				if ( count >= mask_)
				{
					capacity_ <<= 1;
					shared_array< T > array( new T[capacity_]);
					for ( int i( 0); i != count; ++i)
						array[i] = array_[(i + head) & mask_];
					array_.swap( array);
					head_idx_ = 0;
					tail_idx_ = tail = count;
					mask_ = (mask_ << 1) | 1;
				}
				array_[tail & mask_] = t;
				tail_idx_ = tail + 1;
			}
		}
	
		bool try_take( T & t)
		{
			uint32_t tail( tail_idx_);
			if ( tail == 0)
				return false;
			tail -= 1;
			interprocess::detail::atomic_write32( & tail_idx_, tail);
			if ( head_idx_ <= tail)
			{
				t = array_[tail & mask_];
				return true;
			}
			else
			{
				unique_lock< recursive_mutex > lk( mtx_);
				if ( head_idx_ <= tail)
				{
					t = array_[tail & mask_];
					return true;
				}
				else
				{
					tail_idx_ = tail + 1;
					return false;
				}
			}
		}
	
		bool try_steal( T & t)
		{
			recursive_mutex::scoped_try_lock lk( mtx_);
			if ( lk.owns_lock() )
			{
				uint32_t head( head_idx_);
				interprocess::detail::atomic_write32( & head_idx_, head + 1);
				if ( head < tail_idx_)
				{
					t = array_[head & mask_];
					return true;
				}
				else
				{
					head_idx_ = head;
					return false;
				}
			}
			return false;
		}
	};

	shared_ptr< impl >	impl_;

public:
	wsq()
	: impl_( new impl() )
	{}

	bool empty() const
	{ return impl_->empty(); }

	std::size_t size() const
	{ return impl_->size(); }

	void put( T const& t)
	{ impl_->put( t); }

	bool try_take( T & t)
	{ return impl_->try_take( t); }

	bool try_steal( T & t)
	{ return impl_->try_steal( t); }

	void swap( wsq & queue)
	{
		if ( this == & queue) return;
		impl_.swap( queue.impl_);
	}
};
} } }

#endif // BOOST_TP_DETAIL_WSQ_H

