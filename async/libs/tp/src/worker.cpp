#include "boost/tp/detail/worker.hpp"

namespace boost {
namespace tp {
namespace detail
{
thread_specific_ptr< worker > worker::tss_;

worker::impl::impl( function< void() > const& fn)
:
thrd_( new thread( fn) ),
wsq_(),
shtdwn_sem_( 0),
shtdwn_now_sem_( 0),
shtdwn_( false),
scns_( 0)
{ BOOST_ASSERT( ! fn.empty() ); }

const shared_ptr< thread >
worker::impl::thrd() const
{ return thrd_; }

const thread::id
worker::impl::get_id() const
{ return thrd_->get_id(); }

void
worker::impl::join() const
{ thrd_->join(); }

void
worker::impl::interrupt() const
{ thrd_->interrupt(); }

void
worker::impl::put(
	callable const& ca,
	interrupter const& intr)
{
	BOOST_ASSERT( ! ca.empty() );
	wsq_.put( std::make_pair( ca, intr) );
}

bool
worker::impl::try_take(
	callable & ca,
	interrupter & intr)
{
	item itm;
	bool result( wsq_.try_take( itm) );
	if ( result)
	{
		ca = itm.first;
		intr = itm.second;
	}
	return result;
}

bool
worker::impl::try_steal(
	callable & ca,
	interrupter & intr)
{
	item itm;
	bool result( wsq_.try_steal( itm) );
	if ( result)
	{
		ca = itm.first;
		intr = itm.second;
	}
	return result;
}

bool
worker::impl::empty()
{ return wsq_.empty(); }

void
worker::impl::signal_shutdown()
{ shtdwn_sem_.post(); }

void
worker::impl::signal_shutdown_now()
{ shtdwn_now_sem_.post(); }

bool
worker::impl::shutdown()
{
	if ( ! shtdwn_)
		shtdwn_ = shtdwn_sem_.try_wait();
	return shtdwn_;
}

bool
worker::impl::shutdown_now()
{ return shtdwn_now_sem_.try_wait(); }

std::size_t
worker::impl::scanns() const
{ return scns_; }

void
worker::impl::increment_scanns()
{ ++scns_; }

void
worker::impl::reset_scanns()
{ scns_ = 0; }

const shared_ptr< thread >
worker::thrd() const
{ return impl_->thrd(); }

const thread::id
worker::get_id() const
{ return impl_->get_id(); }

void
worker::join() const
{ impl_->join(); }

void
worker::interrupt() const
{ impl_->interrupt(); }

void
worker::put(
	callable const& ca,
	interrupter const& intr)
{ impl_->put( ca, intr); }

bool
worker::try_take(
	callable & ca,
	interrupter & intr)
{ return impl_->try_take( ca, intr); }

bool
worker::try_steal(
	callable & ca,
	interrupter & intr)
{ return impl_->try_steal( ca, intr); }

bool
worker::empty() const
{ return impl_->empty(); }

void
worker::signal_shutdown()
{ impl_->signal_shutdown(); }

void
worker::signal_shutdown_now()
{ impl_->signal_shutdown_now(); }

bool
worker::shutdown()
{ return impl_->shutdown(); }

bool
worker::shutdown_now()
{ return impl_->shutdown_now(); }

std::size_t
worker::scanns() const
{ return impl_->scanns(); }

void
worker::increment_scanns()
{ impl_->increment_scanns(); }

void
worker::reset_scanns()
{ impl_->reset_scanns(); }

void
worker::reschedule_until( function< bool() > const& pred)
{ return impl_->reschedule_until( pred); }

worker *
worker::tss_get()
{ return worker::tss_.get(); }

void
worker::tss_reset( worker * w)
{ worker::tss_.reset( w); }

} } }

