
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/detail/wsq.hpp"

#include <boost/thread/locks.hpp>

#include <boost/task/detail/atomic.hpp>

namespace boost { namespace task {
namespace detail
{

wsq::wsq()
:
initial_size_( 32),
array_( new callable[ initial_size_]),
capacity_( initial_size_),
mask_( initial_size_ - 1),
head_idx_( 0),
tail_idx_( 0),
mtx_()
{}

bool
wsq::empty() const
{ return head_idx_ >= tail_idx_; }

std::size_t
wsq::size() const
{ return tail_idx_ - head_idx_; }

void
wsq::put( callable const& ca)
{
	uint32_t tail( tail_idx_);
	if ( tail <= head_idx_ + mask_)
	{
		array_[tail & mask_] = ca;
		tail_idx_ = tail + 1;
	}
	else
	{
		lock_guard< recursive_mutex > lk( mtx_);
		uint32_t head( head_idx_);
		int count( size() );

		if ( count >= mask_)
		{
			capacity_ <<= 1;
			shared_array< callable > array( new callable[capacity_]);
			for ( int i( 0); i != count; ++i)
				array[i] = array_[(i + head) & mask_];
			array_.swap( array);
			head_idx_ = 0;
			tail_idx_ = tail = count;
			mask_ = (mask_ << 1) | 1;
		}
		array_[tail & mask_] = ca;
		tail_idx_ = tail + 1;
	}
}

bool
wsq::try_take( callable & ca)
{
	uint32_t tail( tail_idx_);
	if ( tail == 0)
		return false;
	tail -= 1;
	atomic_exchange( & tail_idx_, tail);
	if ( head_idx_ <= tail)
	{
		ca.swap( array_[tail & mask_]);
		return true;
	}
	else
	{
		lock_guard< recursive_mutex > lk( mtx_);
		if ( head_idx_ <= tail)
		{
			ca.swap( array_[tail & mask_]);
			return true;
		}
		else
		{
			tail_idx_ = tail + 1;
			return false;
		}
	}
}

bool
wsq::try_steal( callable & ca)
{
	recursive_mutex::scoped_try_lock lk( mtx_);
	if ( lk.owns_lock() )
	{
		uint32_t head( head_idx_);
		atomic_exchange( & head_idx_, head + 1);
		if ( head < tail_idx_)
		{
			ca.swap( array_[head & mask_]);
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

}}}
