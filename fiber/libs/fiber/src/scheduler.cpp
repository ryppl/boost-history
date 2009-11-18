
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fiber/scheduler.hpp>

#include <boost/fiber/detail/move.hpp>
#include <boost/fiber/detail/scheduler_impl.hpp>
#include <boost/fiber/exceptions.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fibers {

scheduler::impl_t scheduler::impl_;

bool
scheduler::runs_as_fiber()
{ return impl_.get() != 0; }

fiber::id
scheduler::get_id()
{
	detail::scheduler_impl * impl( impl_.get() );
	if ( ! impl) throw fiber_error("not a fiber");
	return impl->get_id();
}

void
scheduler::yield()
{
	detail::scheduler_impl * impl( impl_.get() );
	if ( ! impl) throw fiber_error("not a fiber");
	impl->yield();
}

void
scheduler::cancel()
{
	detail::scheduler_impl * impl( impl_.get() );
	if ( ! impl) throw fiber_error("not a fiber");
	impl->cancel();
}

void
scheduler::suspend()
{
	detail::scheduler_impl * impl( impl_.get() );
	if ( ! impl) throw fiber_error("not a fiber");
	impl->suspend();
}

void
scheduler::interrupt()
{
	detail::scheduler_impl * impl( impl_.get() );
	if ( ! impl) throw fiber_error("not a fiber");
	impl->interrupt();
}

bool
scheduler::interruption_requested()
{
	detail::scheduler_impl * impl( impl_.get() );
	if ( ! impl) throw fiber_error("not a fiber");
	return impl->interruption_requested();
}

bool
scheduler::interruption_enabled()
{
	detail::scheduler_impl * impl( impl_.get() );
	if ( ! impl) throw fiber_error("not a fiber");
	return impl->interruption_enabled();
}

detail::fiber_interrupt_t &
scheduler::interrupt_flags()
{
	detail::scheduler_impl * impl( impl_.get() );
	if ( ! impl) throw fiber_error("not a fiber");
	return impl->interrupt_flags();
}

int
scheduler::priority()
{
	detail::scheduler_impl * impl( impl_.get() );
	if ( ! impl) throw fiber_error("not a fiber");
	return impl->priority();
}

void
scheduler::priority( int prio)
{
	detail::scheduler_impl * impl( impl_.get() );
	if ( ! impl) throw fiber_error("not a fiber");
	impl->priority( prio);
}

void
scheduler::at_exit( callable_t ca)
{
	detail::scheduler_impl * impl( impl_.get() );
	if ( ! impl) throw fiber_error("not a fiber");
	impl->at_exit( ca);
}

void
scheduler::interrupt( fiber::id const& id)
{
	detail::scheduler_impl * impl( impl_.get() );
	if ( ! impl) throw fiber_error("not a fiber");
	impl->interrupt( id);
}

void
scheduler::cancel( fiber::id const& id)
{
	detail::scheduler_impl * impl( impl_.get() );
	if ( ! impl) throw fiber_error("not a fiber");
	impl->cancel( id);
}

void
scheduler::suspend( fiber::id const& id)
{
	detail::scheduler_impl * impl( impl_.get() );
	if ( ! impl) throw fiber_error("not a fiber");
	impl->suspend( id);
}

void
scheduler::resume( fiber::id const& id)
{
	detail::scheduler_impl * impl( impl_.get() );
	if ( ! impl) throw fiber_error("not a fiber");
	impl->resume( id);
}

void
scheduler::join( fiber::id const& id)
{
	detail::scheduler_impl * impl( impl_.get() );
	if ( ! impl) throw fiber_error("not a fiber");
	impl->join( id);
}

void
scheduler::reschedule( fiber::id const& id)
{
	detail::scheduler_impl * impl( impl_.get() );
	if ( ! impl) throw fiber_error("not a fiber");
	impl->reschedule( id);
}

detail::scheduler_impl *
scheduler::access_()
{
	if ( ! impl_.get() )
		impl_.reset( new detail::scheduler_impl() );
	return impl_.get();
}

scheduler::~scheduler()
{ impl_.reset(); }

bool
scheduler::run()
{ return access_()->run(); }

bool
scheduler::empty()
{ return access_()->empty(); }

std::size_t
scheduler::size()
{ return access_()->size(); }

void
scheduler::submit_fiber( fiber f)
{ access_()->add( f); }

}}

#include <boost/config/abi_suffix.hpp>
