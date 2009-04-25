
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/detail/worker.hpp"

namespace boost { namespace task
{
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
worker::put( pool_callable const& ca)
{ impl_->put( ca); }

bool
worker::try_take( pool_callable & ca)
{ return impl_->try_take( ca); }

bool
worker::try_steal( pool_callable & ca)
{ return impl_->try_steal( ca); }

void
worker::reschedule_until( function< bool() > const& pred)
{ return impl_->reschedule_until( pred); }

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

