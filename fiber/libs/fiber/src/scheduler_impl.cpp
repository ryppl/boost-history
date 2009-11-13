
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

#define HAS_STATE_MASTER( state) \
	( state & STATE_MASTER) != 0

#define HAS_STATE_NOT_STARTED( state) \
	( state & STATE_NOT_STARTED) != 0

#define HAS_STATE_READY( state) \
	( state & STATE_READY) != 0

#define HAS_STATE_RUNNING( state) \
	( state & STATE_RUNNING) != 0

#define HAS_STATE_SUSPENDED( state) \
	( state & STATE_SUSPENDED) != 0

#define HAS_STATE_WAITING( state) \
	( state & STATE_WAITING) != 0

#define HAS_STATE_TERMINATED( state) \
	( state & STATE_TERMINATED) != 0

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
	BOOST_ASSERT( ! HAS_STATE_MASTER( f.info_->state) );
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
	BOOST_ASSERT( ! HAS_STATE_MASTER( fibers_[f_id_].info_->state) );
	BOOST_ASSERT( STATE_RUNNING == fibers_[f_id_].info_->state);
	fibers_[f_id_].info_->state = STATE_READY;
	runnable_fibers_.push_back( f_id_);
	fibers_[f_id_].switch_to_( master_);
}

void
scheduler_impl::cancel_active_fiber()
{
	BOOST_ASSERT( ! HAS_STATE_MASTER( fibers_[f_id_].info_->state) );
	BOOST_ASSERT( STATE_RUNNING == fibers_[f_id_].info_->state);
	fibers_[f_id_].info_->state = STATE_TERMINATED;
	terminated_fibers_.push( f_id_);
	fibers_[f_id_].switch_to_( master_);
}

void
scheduler_impl::suspend_active_fiber()
{
	BOOST_ASSERT( ! HAS_STATE_MASTER( fibers_[f_id_].info_->state) );
	BOOST_ASSERT( STATE_RUNNING == fibers_[f_id_].info_->state);
	fibers_[f_id_].info_->state |= STATE_SUSPENDED;
	fibers_[f_id_].switch_to_( master_);
}

void
scheduler_impl::cancel_fiber( fiber::id const& f_id)
{
	container::iterator i = fibers_.find( f_id);
	if ( i == fibers_.end() ) return;
	fiber f( i->second);
	BOOST_ASSERT( f);
	BOOST_ASSERT( ! HAS_STATE_MASTER( f.info_->state) );
	
	if ( HAS_STATE_TERMINATED( f.info_->state) ||
	     HAS_STATE_NOT_STARTED( f.info_->state) )
		return;

	if ( HAS_STATE_READY( f.info_->state) )
	{
		f.info_->state = STATE_TERMINATED;
		runnable_fibers_.remove( f.get_id() );
		terminated_fibers_.push( f_id);	
	}
	else if ( HAS_STATE_RUNNING( f.info_->state) )
	{
		BOOST_ASSERT( f_id == f.get_id() );
		f.info_->state = STATE_TERMINATED;
		terminated_fibers_.push( f_id);
		f.switch_to_( master_);
	}
	else if ( HAS_STATE_WAITING( f.info_->state) )
	{
		f.info_->state = STATE_TERMINATED;
		// TODO: remove from waiting-queue
		terminated_fibers_.push( f_id);	
	}
	else
		BOOST_ASSERT( ! "should never reached");
}

void
scheduler_impl::suspend_fiber( fiber::id const& f_id)
{
	container::iterator i = fibers_.find( f_id);
	if ( i == fibers_.end() ) return;
	fiber f( i->second);
	BOOST_ASSERT( f);
	BOOST_ASSERT( ! HAS_STATE_MASTER( f.info_->state) );
	
	if ( HAS_STATE_TERMINATED( f.info_->state) ||
	     HAS_STATE_NOT_STARTED( f.info_->state) )
		return;

	if ( HAS_STATE_READY( f.info_->state) )
	{
		f.info_->state |= STATE_SUSPENDED;
		runnable_fibers_.remove( f.get_id() );
	}
	else if ( HAS_STATE_RUNNING( f.info_->state) )
	{
		BOOST_ASSERT( f_id == f.get_id() );
		f.info_->state |= STATE_SUSPENDED;
		f.switch_to_( master_);
	}
	else if ( HAS_STATE_WAITING( f.info_->state) )
	{
		f.info_->state |= STATE_SUSPENDED;
		// TODO: remove from waiting-queue
	}
	else
		BOOST_ASSERT( ! "should never reached");
}

void
scheduler_impl::resume_fiber( fiber::id const& f_id)
{
	container::iterator i = fibers_.find( f_id);
	if ( i == fibers_.end() ) return;
	fiber f( i->second);
	BOOST_ASSERT( f);
	BOOST_ASSERT( ! HAS_STATE_MASTER( f.info_->state) );

	if ( HAS_STATE_SUSPENDED( f.info_->state) )
	{
		f.info_->state &= ~STATE_SUSPENDED;
		switch ( f.info_->state)
		{
		case STATE_READY:
		case STATE_RUNNING:
			f.info_->state = STATE_READY;
			runnable_fibers_.push_back( f.get_id() );
			break;
		case STATE_WAITING:
		// TODO: put it into the waiting-queue
			break;
		default:
			BOOST_ASSERT( ! "should never reached");
		}
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

#undef HAS_STATE_MASTER
#undef HAS_STATE_NOT_STARTED
#undef HAS_STATE_READY
#undef HAS_STATE_RUNNING
#undef HAS_STATE_SUSPENDED
#undef HAS_STATE_WAITING
#undef HAS_STATE_TERMINATED

}}}

#include <boost/config/abi_suffix.hpp>
