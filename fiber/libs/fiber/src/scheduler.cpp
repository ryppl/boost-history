
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fiber/scheduler.hpp>

#include <boost/fiber/detail/move.hpp>
#include <boost/fiber/exceptions.hpp>
#include <boost/fiber/rrp.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fibers {

scheduler::tss_policy_t scheduler::impl_;

bool
scheduler::runs_as_fiber()
{ return impl_.get() != 0; }

fiber::id
scheduler::get_id()
{
	policy * impl( impl_.get() );
	if (  ! impl) throw fiber_error("not a fiber");
	return impl->active_fiber();
}

void
scheduler::fiber_yield()
{
	policy * impl( impl_.get() );
	if (  ! impl) throw fiber_error("not a fiber");
	impl->yield_active_fiber();
}

void
scheduler::fiber_exit()
{
	policy * impl( impl_.get() );
	if (  ! impl) throw fiber_error("not a fiber");
	impl->exit_active_fiber();
}

void
scheduler::fiber_failed()
{
	policy * impl( impl_.get() );
	if (  ! impl) throw fiber_error("not a fiber");
	impl->exit_active_fiber();
}

scheduler::scheduler( policy_t pol) :
	pol_( pol)
{}

policy *
scheduler::access_()
{
	if ( ! impl_.get() )
	{
		fiber::convert_thread_to_fiber();
		switch ( pol_)
		{
		case round_robin_policy:
			impl_.reset( new rrp() );
			break;
		default:
			throw scheduler_error("invalid scheduling policy");
		}
	}
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
{
	access_()->add_fiber(
		std::auto_ptr< fiber >(
			new fiber(
				boost::move( f) ) ) );
}

}}

#include <boost/config/abi_suffix.hpp>
