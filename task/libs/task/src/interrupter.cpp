
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/detail/interrupter.hpp"

#include <boost/assert.hpp>

namespace boost { namespace task
{
namespace detail
{

void
interrupter::impl::reset_( shared_ptr< thread > const& thrd)
{
	thrd_ = thrd;
	BOOST_ASSERT( thrd_);
	if ( requested_)
		if ( thrd_) thrd_->interrupt();
}

void
interrupter::impl::reset_()
{
	thrd_.reset();
	try
	{ this_thread::interruption_point(); }
	catch ( thread_interrupted const&)
	{}
	BOOST_ASSERT( ! this_thread::interruption_requested() );
}

void
interrupter::impl::interrupt_()
{
	if ( ! requested_)
	{
		requested_ = true;
		if ( thrd_) thrd_->interrupt();
	}
}

interrupter::impl::impl()
:
requested_( false),
mtx_(),
thrd_()
{}

void
interrupter::impl::reset( shared_ptr< thread > const& thrd)
{
	lock_guard< mutex > lk( mtx_);
	reset_( thrd);
}

void
interrupter::impl::reset()
{
	lock_guard< mutex > lk( mtx_);
	reset_();
}

void
interrupter::impl::interrupt()
{
	lock_guard< mutex > lk( mtx_);
	interrupt_();
}

bool
interrupter::impl::interruption_requested()
{
	lock_guard< mutex > lk( mtx_);
	return requested_;
}

interrupter::interrupter()
: impl_( new impl() )
{}

void
interrupter::reset( shared_ptr< thread > const& thrd)
{ impl_->reset( thrd); }

void
interrupter::reset()
{ impl_->reset(); }

void
interrupter::interrupt()
{ impl_->interrupt(); }

bool
interrupter::interruption_requested()
{ return impl_->interruption_requested(); }

void
interrupter::swap( interrupter & other)
{ impl_.swap( other.impl_); }

}}}
