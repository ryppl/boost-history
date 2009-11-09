
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
scheduler::yield()
{
	policy * impl( impl_.get() );
	if (  ! impl) throw fiber_error("not a fiber");
	impl->yield_active_fiber();
}

void
scheduler::exit()
{
	policy * impl( impl_.get() );
	if (  ! impl) throw fiber_error("not a fiber");
	impl->exit_active_fiber();
}

void
scheduler::failed()
{
	policy * impl( impl_.get() );
	if (  ! impl) throw fiber_error("not a fiber");
	impl->exit_active_fiber();
}

policy *
scheduler::access_()
{
	if ( ! impl_.get() )
	{
		fiber::convert_thread_to_fiber();
		impl_.reset( new rrp() );
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

}}

#include <boost/config/abi_suffix.hpp>
