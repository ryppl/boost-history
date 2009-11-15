
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
{ return impl_; }

fiber::id
scheduler::active_fiber()
{
	if ( ! impl_) throw fiber_error("not a fiber");
	return impl_->active_fiber();
}

void
scheduler::yield_active_fiber()
{
	if ( ! impl_) throw fiber_error("not a fiber");
	impl_->yield_active_fiber();
}

void
scheduler::cancel_active_fiber()
{
	if ( ! impl_) throw fiber_error("not a fiber");
	impl_->cancel_active_fiber();
}

void
scheduler::suspend_active_fiber()
{
	if ( ! impl_) throw fiber_error("not a fiber");
	impl_->suspend_active_fiber();
}

void
scheduler::cancel_fiber( fiber::id const& id)
{
	if ( ! impl_) throw fiber_error("not a fiber");
	impl_->cancel_fiber( id);
}

void
scheduler::suspend_fiber( fiber::id const& id)
{
	if ( ! impl_) throw fiber_error("not a fiber");
	impl_->suspend_fiber( id);
}

void
scheduler::resume_fiber( fiber::id const& id)
{
	if ( ! impl_) throw fiber_error("not a fiber");
	impl_->resume_fiber( id);
}

int
scheduler::priority( fiber::id const& id)
{
	if ( ! impl_) throw fiber_error("not a fiber");
	return impl_->priority( id);
}

void
scheduler::priority( fiber::id const& id, int prio)
{
	if ( ! impl_) throw fiber_error("not a fiber");
	impl_->priority( id, prio);
	re_schedule( id);
}

void
scheduler::re_schedule( fiber::id const& id)
{ impl_->re_schedule( id); }

void
scheduler::join( fiber::id const& id)
{ impl_->join( id); }

scheduler::scheduler()
{
	fiber::convert_thread_to_fiber();
	impl_.reset(
		new detail::scheduler_impl() );
}

scheduler::~scheduler()
{ fiber::convert_fiber_to_thread(); }

bool
scheduler::run()
{ return impl_->run(); }

bool
scheduler::empty()
{ return impl_->empty(); }

std::size_t
scheduler::size()
{ return impl_->size(); }

void
scheduler::submit_fiber( fiber f)
{ impl_->add_fiber( f); }

}}

#include <boost/config/abi_suffix.hpp>
