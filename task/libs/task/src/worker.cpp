
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/detail/worker.hpp"

namespace boost { namespace task {
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
worker::put( callable const& ca)
{ impl_->put( ca); }

bool
worker::try_steal( callable & ca)
{ return impl_->try_steal( ca); }

void
worker::run()
{
	// FIXME: ugly
	worker::tss_.reset( new worker( * this) );
	impl_->run();
}

void
worker::reschedule_until( function< bool() > const& pred)
{ impl_->reschedule_until( pred); }

bool
worker::block()
{ return impl_->block(); }

worker *
worker::tss_get()
{ return worker::tss_.get(); }

}}}
