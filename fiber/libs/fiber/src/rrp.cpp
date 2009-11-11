
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fiber/rrp.hpp>

#include <utility>

#include <boost/fiber/detail/fiber_info.hpp>
#include <boost/fiber/detail/move.hpp>
#include <boost/fiber/exceptions.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fibers {

rrp::rrp() :
	master_(
		detail::fiber_info_base::ptr_t(
			new detail::fiber_info_default() ) ),
	f_id_(),
	fibers_(),
	runnable_fibers_(),
	dead_fibers_()
{}

void
rrp::add_fiber( std::auto_ptr< fiber > f)
{
	fiber::id id( f->get_id() );
	std::pair< ptr_map< fiber::id, fiber >::iterator, bool > result(
		fibers_.insert( id, f) );
	if ( ! result.second) throw scheduler_error("inserting fiber failed");
	runnable_fibers_.push( result.first->first);
}

fiber::id
rrp::active_fiber()
{ return f_id_; }

void
rrp::yield_active_fiber()
{
	runnable_fibers_.push( f_id_);
	fibers_[f_id_].switch_to( master_);
}

void
rrp::exit_active_fiber()
{
	dead_fibers_.push( f_id_);
	fibers_[f_id_].switch_to( master_);
}

bool
rrp::run()
{
	bool result( false);
	if ( ! runnable_fibers_.empty() )
	{
		f_id_ = runnable_fibers_.front();
		master_.switch_to( fibers_[f_id_]);
		runnable_fibers_.pop();
		result = true;
	}
	if ( ! dead_fibers_.empty() )
	{
		fibers_.erase( dead_fibers_.front() );
		dead_fibers_.pop();
		result = true;
	}
	return result;
}

bool
rrp::empty()
{ return fibers_.empty(); }

std::size_t
rrp::size()
{ return fibers_.size(); }

}}

#include <boost/config/abi_suffix.hpp>
