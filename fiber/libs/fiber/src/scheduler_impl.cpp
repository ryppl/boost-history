
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
		fiber_info_base::ptr_t(
			new fiber_info_default() ) ),
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
	BOOST_ASSERT( STATE_NOT_STARTED == f.info_->state);
	f.info_->state = STATE_READY;
	std::pair< std::map< fiber::id, fiber >::iterator, bool > result(
		fibers_.insert( std::make_pair( id, f) ) );
	if ( ! result.second) throw scheduler_error("inserting fiber failed");
	runnable_fibers_.push_back( result.first->first);
}

fiber::id
scheduler_impl::active_fiber()
{ return f_id_; }

void
scheduler_impl::yield_active_fiber()
{
	BOOST_ASSERT( STATE_RUNNING == fibers_[f_id_].info_->state);
	fibers_[f_id_].info_->state = STATE_READY;
	runnable_fibers_.push_back( f_id_);
	fibers_[f_id_].switch_to_( master_);
}

void
scheduler_impl::terminate_active_fiber()
{
	BOOST_ASSERT( STATE_RUNNING == fibers_[f_id_].info_->state);
	fibers_[f_id_].info_->state = STATE_TERMINATED;
	terminated_fibers_.push_back( f_id_);
	fibers_[f_id_].switch_to_( master_);
}

void
scheduler_impl::cancel_fiber( fiber::id const& f_id)
{
	container::iterator i = fibers_.find( f_id);
	if ( i == fibers_.end() ) return;
	fiber f( i->second);
	BOOST_ASSERT( f);
	BOOST_ASSERT( STATE_MASTER != f.info_->state);
	if ( STATE_TERMINATED == f.info_->state) return;
	else if ( STATE_NOT_STARTED == f.info_->state)
	{
		f.info_->state = STATE_TERMINATED;
		terminated_fibers_.push_back( f_id);	
	}
	else if ( STATE_READY == f.info_->state)
	{
		f.info_->state = STATE_TERMINATED;
		runnable_fibers_.remove( f.get_id() );
		terminated_fibers_.push_back( f_id);	
	}
	else if ( STATE_RUNNING == f.info_->state)
	{
		f.info_->state = STATE_TERMINATED;
		terminated_fibers_.push_back( f_id);
		f.switch_to_( master_);
	}
	else 
	{
		// TODO: remove waiting fiber/suspended fiber
		f.info_->state = STATE_TERMINATED;
		terminated_fibers_.push_back( f_id);	
	}
}

bool
scheduler_impl::run()
{
	bool result( false);
	if ( ! runnable_fibers_.empty() )
	{
		f_id_ = runnable_fibers_.front();
		BOOST_ASSERT( fibers_[f_id_].info_->state == STATE_READY);
		fibers_[f_id_].info_->state = STATE_RUNNING;
		master_.switch_to_( fibers_[f_id_]);
		runnable_fibers_.pop_front();
		result = true;
	}
	while ( ! terminated_fibers_.empty() )
	{
		BOOST_ASSERT( STATE_TERMINATED == fibers_[terminated_fibers_.front()].info_->state);	
		fibers_.erase( terminated_fibers_.front() );
		terminated_fibers_.pop_front();
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
