
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fiber/scheduler.hpp>

#include <boost/fiber/exceptions.hpp>
#include <boost/fiber/rrp.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fiber {

scheduler::policy_ptr_t scheduler::policy_;

bool
scheduler::runs_as_fiber()
{ return policy_.get() != 0; }

fiber::id
scheduler::get_id()
{
	policy * po( policy_.get() );
	if ( ! po) throw fiber_error("not a fiber");
	return po->active_fiber();
}

void
scheduler::yield()
{
	policy * po( policy_.get() );
	if ( ! po) throw fiber_error("not a fiber");
	po->yield_active_fiber();
}

void
scheduler::exit()
{
	policy * po( policy_.get() );
	if ( ! po) throw fiber_error("not a fiber");
	po->exit_active_fiber();
}

scheduler::scheduler()
{}

policy *
scheduler::access_()
{
	if ( ! policy_.get() )
	{
		fiber::convert_thread_to_fiber();
		policy_.reset( new rrp() );
	}
	return policy_.get();
}

bool
scheduler::run()
{ return access_()->run(); }

bool
scheduler::empty()
{ return access_()->empty(); }

std::size_t
scheduler::size()
{ return access_()->size(); }

}}

#include <boost/config/abi_suffix.hpp>
