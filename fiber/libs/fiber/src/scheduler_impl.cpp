
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fiber/detail/scheduler_impl.hpp>

#include <utility>

#include <boost/assert.hpp>

#include <boost/fiber/detail/fiber_info.hpp>
#include <boost/fiber/detail/move.hpp>
#include <boost/fiber/detail/fiber_state.hpp>
#include <boost/fiber/exceptions.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fibers {
namespace detail {

scheduler_impl::scheduler_impl() :
	master_(
		detail::fiber_info_base::ptr_t(
			new detail::fiber_info_default() ) ),
	f_id_(),
	fibers_(),
	runnable_fibers_(),
	terminated_fibers_()
{}

void
scheduler_impl::add_fiber( fiber f)
{
	if ( ! f) throw fiber_moved();
	fiber::id id( f.get_id() );
	BOOST_ASSERT( f.info_->state == detail::STATE_NOT_STARTED);
	f.info_->state = detail::STATE_READY;
	std::pair< std::map< fiber::id, fiber >::iterator, bool > result(
		fibers_.insert( std::make_pair( id, f) ) );
	if ( ! result.second) throw scheduler_error("inserting fiber failed");
	runnable_fibers_.push( result.first->first);
}

fiber::id
scheduler_impl::active_fiber()
{ return f_id_; }

void
scheduler_impl::yield_active_fiber()
{
	BOOST_ASSERT( fibers_[f_id_].info_->state == detail::STATE_RUNNING);
	fibers_[f_id_].info_->state = detail::STATE_READY;
	runnable_fibers_.push( f_id_);
	fibers_[f_id_].switch_to( master_);
}

void
scheduler_impl::terminate_active_fiber()
{
	BOOST_ASSERT( fibers_[f_id_].info_->state == detail::STATE_RUNNING);
	fibers_[f_id_].info_->state = detail::STATE_TERMINATED;
	terminated_fibers_.push( f_id_);
	fibers_[f_id_].switch_to( master_);
}

bool
scheduler_impl::run()
{
	bool result( false);
	if ( ! runnable_fibers_.empty() )
	{
		f_id_ = runnable_fibers_.front();
		BOOST_ASSERT( fibers_[f_id_].info_->state == detail::STATE_READY);
		fibers_[f_id_].info_->state = detail::STATE_RUNNING;
		master_.switch_to( fibers_[f_id_]);
		runnable_fibers_.pop();
		result = true;
	}
	while ( ! terminated_fibers_.empty() )
	{
		BOOST_ASSERT( fibers_[terminated_fibers_.front()].info_->state == detail::STATE_TERMINATED);	
		fibers_.erase( terminated_fibers_.front() );
		terminated_fibers_.pop();
		result = true;
	}
	return result;
}

bool
scheduler_impl::empty()
{ return fibers_.empty(); }

std::size_t
scheduler_impl::size()
{ return fibers_.size(); }

}}}

#include <boost/config/abi_suffix.hpp>
