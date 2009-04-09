#include "boost/tp/detail/worker.hpp"

namespace boost {
namespace tp {
namespace detail
{
thread_specific_ptr< worker > worker::tss_;

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
worker::signal_shutdown()
{ impl_->signal_shutdown(); }

void
worker::signal_shutdown_now()
{ impl_->signal_shutdown_now(); }

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

void
worker::reschedule_until( function< bool() > const& pred)
{ return impl_->schedule_until( pred); }

void
worker::run()
{
	worker::tss_.reset( new worker( * this) );
	impl_->run();
}

worker *
worker::tss_get()
{ return worker::tss_.get(); }
} } }

