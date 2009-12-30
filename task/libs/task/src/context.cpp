
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/context.hpp"

#include <boost/assert.hpp>

namespace boost {
namespace tasks {

void
context::impl::reset_( shared_ptr< thread > const& thrd)
{
	thrd_ = thrd;
	BOOST_ASSERT( thrd_);
	if ( requested_)
		if ( thrd_) thrd_->interrupt();
}

void
context::impl::interrupt_()
{
	if ( ! requested_)
	{
		requested_ = true;
		if ( thrd_) thrd_->interrupt();
	}
}

context::impl::impl() :
	use_count_( 0),
	requested_( false),
	mtx_(),
	thrd_()
{}

void
context::impl::reset( shared_ptr< thread > const& thrd)
{
	lock_guard< mutex > lk( mtx_);
	reset_( thrd);
}

void
context::impl::interrupt()
{
	lock_guard< mutex > lk( mtx_);
	interrupt_();
}

bool
context::impl::interruption_requested()
{
	lock_guard< mutex > lk( mtx_);
	return requested_;
}

context::context() :
	impl_( new impl() )
{}

void
context::reset( shared_ptr< thread > const& thrd)
{ impl_->reset( thrd); }

void
context::interrupt()
{ impl_->interrupt(); }

bool
context::interruption_requested()
{ return impl_->interruption_requested(); }

void
context::swap( context & other)
{ impl_.swap( other.impl_); }

}}
