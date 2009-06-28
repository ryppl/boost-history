
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
interrupter::impl::set_( shared_ptr< thread > const& thrd)
{
	thrd_ = thrd;
	BOOST_ASSERT( thrd_);
	if ( interruption_requested_)
		if ( thrd_) thrd_->interrupt();
}

void
interrupter::impl::reset_()
{
	try
	{ this_thread::interruption_point(); }
	catch ( thread_interrupted const&)
	{}
	thrd_.reset();
	BOOST_ASSERT( ! this_thread::interruption_requested() );
}

void
interrupter::impl::interrupt_()
{
	if ( ! interruption_requested_)
	{
		interruption_requested_ = true;
		if ( thrd_) thrd_->interrupt();
	}
}

interrupter::impl::impl()
:
interruption_requested_( false),
mtx_(),
thrd_()
{}

void
interrupter::impl::set( shared_ptr< thread > const& thrd)
{
	unique_lock< mutex > lk( mtx_);
	set_( thrd);
}

void
interrupter::impl::reset()
{
	unique_lock< mutex > lk( mtx_);
	reset_();
}

void
interrupter::impl::interrupt()
{
	unique_lock< mutex > lk( mtx_);
	interrupt_();
}

bool
interrupter::impl::interruption_requested()
{
	unique_lock< mutex > lk( mtx_);
	return interruption_requested_;
}

interrupter::interrupter()
: impl_( new impl() )
{}

void
interrupter::set( shared_ptr< thread > const& thrd)
{ impl_->set( thrd); }

void
interrupter::reset()
{ impl_->reset(); }

void
interrupter::interrupt()
{ impl_->interrupt(); }

bool
interrupter::interruption_requested()
{ return impl_->interruption_requested(); }

interrupter::scoped_guard::scoped_guard( interrupter & intr, shared_ptr< thread > & thrd)
: intr_( intr)
{ intr_.set( thrd); }

interrupter::scoped_guard::~scoped_guard()
{ intr_.reset(); }
}
} }

