#include "boost/tp/detail/interrupter.hpp"

namespace boost { namespace tp {
namespace detail
{
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
cond_(),
mtx_(),
thrd_()
{}

void
interrupter::impl::set( shared_ptr< thread > const& thrd)
{
	BOOST_ASSERT( thrd);
	unique_lock< mutex > lk( mtx_);
	thrd_ = thrd;
	BOOST_ASSERT( thrd_);
	if ( interruption_requested_) thrd_->interrupt();
}

void
interrupter::impl::reset()
{
	unique_lock< mutex > lk( mtx_);
	thrd_.reset();
	BOOST_ASSERT( ! thrd_);
	try
	{ this_thread::interruption_point(); }
	catch ( thread_interrupted const&)
	{}
	BOOST_ASSERT( ! this_thread::interruption_requested() );
	cond_.notify_all();
}

void
interrupter::impl::interrupt()
{
	unique_lock< mutex > lk( mtx_);
	interrupt_();
}

void
interrupter::impl::interrupt_and_wait()
{
	unique_lock< mutex > lk( mtx_);
	interrupt_();
	cond_.wait( lk);
}

void
interrupter::impl::interrupt_and_wait( system_time const& abs_time)
{
	unique_lock< mutex > lk( mtx_);
	interrupt_();
	cond_.timed_wait( lk, abs_time);
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
{ impl_->reset();  }

void
interrupter::interrupt()
{ impl_->interrupt(); }

void
interrupter::interrupt_and_wait()
{ impl_->interrupt_and_wait(); }

void
interrupter::interrupt_and_wait( system_time const& abs_time)
{ impl_->interrupt_and_wait( abs_time); }

bool
interrupter::interruption_requested()
{ return impl_->interruption_requested(); }
}
} }

