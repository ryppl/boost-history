
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fiber/detail/scheduler_impl.hpp>

#include <utility>

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

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
	master_(),
	active_(),
	fibers_(),
	runnable_fibers_(),
	terminated_fibers_()
{
	fiber::convert_thread_to_fiber();
	master_ = fiber( 
		fiber_info_base::ptr_t(
			new fiber_info_default() ) );
}

scheduler_impl::~scheduler_impl()
{ fiber::convert_fiber_to_thread(); }

void
scheduler_impl::add_fiber( fiber f)
{
	if ( ! f) throw fiber_moved();

	BOOST_ASSERT( ! HAS_STATE_MASTER( f.info_->state) );
	BOOST_ASSERT( STATE_NOT_STARTED == f.info_->state);
	f.info_->state = STATE_READY;
	std::pair< std::map< fiber::id, schedulable >::iterator, bool > result(
		fibers_.insert(
			std::make_pair(
				f.get_id(),
				schedulable( f) ) ) );
	if ( ! result.second) throw scheduler_error("inserting fiber failed");
	runnable_fibers_.push_back( result.first->first);
}

fiber::id
scheduler_impl::id_active_fiber() const
{ return active_.get_id(); }

void
scheduler_impl::yield_active_fiber()
{
	BOOST_ASSERT( ! HAS_STATE_MASTER( active_.info_->state) );
	BOOST_ASSERT( STATE_RUNNING == active_.info_->state);

	active_.info_->state = STATE_READY;

	runnable_fibers_.push_back( active_.get_id() );
	active_.switch_to_( master_);
}

void
scheduler_impl::cancel_active_fiber()
{
	BOOST_ASSERT( ! HAS_STATE_MASTER( active_.info_->state) );
	BOOST_ASSERT( STATE_RUNNING == active_.info_->state);

	active_.info_->state = STATE_TERMINATED;

	terminated_fibers_.push( active_.get_id() );

	BOOST_FOREACH( fiber::id id__, fibers_[active_.get_id()].waiting)
	{
		fiber f__( fibers_[id__].f);
		BOOST_ASSERT( HAS_STATE_WAITING( f__.info_->state) );
		f__.info_->state &= ~STATE_WAITING;
		if ( ( HAS_STATE_READY( f__.info_->state) || HAS_STATE_RUNNING( f__.info_->state) )
		     && ! HAS_STATE_SUSPENDED( f__.info_->state)  )
		{
			f__.info_->state = STATE_READY;
			runnable_fibers_.push_back( f__.get_id() );
		}
	}
	fibers_[active_.get_id()].waiting.clear();

	active_.switch_to_( master_);
}

void
scheduler_impl::suspend_active_fiber()
{
	BOOST_ASSERT( ! HAS_STATE_MASTER( active_.info_->state) );
	BOOST_ASSERT( STATE_RUNNING == active_.info_->state);

	active_.info_->state |= STATE_SUSPENDED;
	active_.switch_to_( master_);
}

void
scheduler_impl::interrupt_active_fiber()
{
	BOOST_ASSERT( ! HAS_STATE_MASTER( active_.info_->state) );
	BOOST_ASSERT( STATE_RUNNING == active_.info_->state);

	if ( INTERRUPTION_ENABLED == active_.info_->interrupt)
		throw fiber_interrupted();
}

bool
scheduler_impl::interruption_requested_active_fiber() const
{
	BOOST_ASSERT( ! HAS_STATE_MASTER( active_.info_->state) );
	BOOST_ASSERT( STATE_RUNNING == active_.info_->state);

	return ( active_.info_->interrupt & detail::INTERRUPTION_ENABLED) != 0;
}

bool
scheduler_impl::interruption_enabled_active_fiber() const
{
	BOOST_ASSERT( ! HAS_STATE_MASTER( active_.info_->state) );
	BOOST_ASSERT( STATE_RUNNING == active_.info_->state);

	return active_.info_->interrupt == detail::INTERRUPTION_ENABLED;
}

fiber_interrupt_t &
scheduler_impl::interrupt_flags_active_fiber()
{
	BOOST_ASSERT( ! HAS_STATE_MASTER( active_.info_->state) );
	BOOST_ASSERT( STATE_RUNNING == active_.info_->state);

	return active_.info_->interrupt;
}

int
scheduler_impl::priority_active_fiber()
{
	BOOST_ASSERT( ! HAS_STATE_MASTER( active_.info_->state) );
	BOOST_ASSERT( STATE_RUNNING == active_.info_->state);

	return active_.priority();
}

void
scheduler_impl::priority_active_fiber( int prio)
{
	BOOST_ASSERT( ! HAS_STATE_MASTER( active_.info_->state) );
	BOOST_ASSERT( STATE_RUNNING == active_.info_->state);

	active_.priority( prio);
	reschedule_fiber( active_.get_id() );
}

void
scheduler_impl::at_exit_active_fiber( callable_t ca)
{
	BOOST_ASSERT( ! HAS_STATE_MASTER( active_.info_->state) );
	BOOST_ASSERT( STATE_RUNNING == active_.info_->state);

	active_.info_->at_exit.push( ca);
}

void
scheduler_impl::cancel_fiber( fiber::id const& id)
{
	container::iterator i = fibers_.find( id);
	if ( i == fibers_.end() ) return;
	fiber f( i->second.f);
	BOOST_ASSERT( f);
	BOOST_ASSERT( ! HAS_STATE_MASTER( f.info_->state) );
	
	if ( HAS_STATE_TERMINATED( f.info_->state) ||
	     HAS_STATE_NOT_STARTED( f.info_->state) )
		return;

	BOOST_FOREACH( fiber::id id__, fibers_[id].waiting)
	{
		fiber f__( fibers_[id__].f);
		BOOST_ASSERT( HAS_STATE_WAITING( f__.info_->state) );
		f__.info_->state &= ~STATE_WAITING;
		if ( ( HAS_STATE_READY( f__.info_->state) || HAS_STATE_RUNNING( f__.info_->state) )
		     && ! HAS_STATE_SUSPENDED( f__.info_->state)  )
		{
			f__.info_->state = STATE_READY;
			runnable_fibers_.push_back( f__.get_id() );
		}
	}
	fibers_[id].waiting.clear();

	if ( HAS_STATE_READY( f.info_->state) )
	{
		f.info_->state = STATE_TERMINATED;
		runnable_fibers_.remove( f.get_id() );
		terminated_fibers_.push( id);	
	}
	else if ( HAS_STATE_RUNNING( f.info_->state) )
	{
		BOOST_ASSERT( active_.get_id() == id);
		f.info_->state = STATE_TERMINATED;
		terminated_fibers_.push( id);
		f.switch_to_( master_);
	}
	else if ( HAS_STATE_WAITING( f.info_->state) )
	{
		f.info_->state = STATE_TERMINATED;
		// TODO: remove from waiting-queue
		terminated_fibers_.push( id);	
	}
	else
		BOOST_ASSERT( ! "should never reached");
}

void
scheduler_impl::suspend_fiber( fiber::id const& id)
{
	container::iterator i = fibers_.find( id);
	if ( i == fibers_.end() ) return;
	fiber f( i->second.f);
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
		BOOST_ASSERT( active_.get_id() == id);
		f.info_->state |= STATE_SUSPENDED;
		f.switch_to_( master_);
	}
	else if ( HAS_STATE_WAITING( f.info_->state) )
		f.info_->state |= STATE_SUSPENDED;
	else
		BOOST_ASSERT( ! "should never reached");
}

void
scheduler_impl::resume_fiber( fiber::id const& id)
{
	container::iterator i = fibers_.find( id);
	if ( i == fibers_.end() ) return;
	fiber f( i->second.f);
	BOOST_ASSERT( f);
	BOOST_ASSERT( ! HAS_STATE_MASTER( f.info_->state) );

	BOOST_ASSERT( active_.get_id() != id);

	if ( HAS_STATE_SUSPENDED( f.info_->state) )
	{
		f.info_->state &= ~STATE_SUSPENDED;
		if ( ( HAS_STATE_READY( f.info_->state) || HAS_STATE_RUNNING( f.info_->state) )
			 && ! HAS_STATE_WAITING( f.info_->state) )
		{
			f.info_->state = STATE_READY;
			runnable_fibers_.push_back( f.get_id() );
		}
	}
}

void
scheduler_impl::reschedule_fiber( fiber::id const& id)
{
	container::iterator i = fibers_.find( id);
	if ( i == fibers_.end() ) return;
	fiber f( i->second.f);
	BOOST_ASSERT( f);
	BOOST_ASSERT( ! HAS_STATE_MASTER( f.info_->state) );

	// TODO: re-schedule fiber == remove from
	// runnable_fibers + re-insert
}

void
scheduler_impl::join_fiber( fiber::id const& id)
{
	container::iterator i = fibers_.find( id);
	if ( i == fibers_.end() ) return;
	fiber f( i->second.f);
	BOOST_ASSERT( f);
	BOOST_ASSERT( ! HAS_STATE_MASTER( f.info_->state) );

	if ( active_.get_id() == id) throw scheduler_error("self-join denied");
	fibers_[id].waiting.push_back( active_.get_id() );
	active_.info_->state |= STATE_WAITING;
	active_.switch_to_( master_);
}

bool
scheduler_impl::run()
{
	bool result( false);
	if ( ! runnable_fibers_.empty() )
	{
		active_ = fibers_[runnable_fibers_.front()].f;
		BOOST_ASSERT( active_.info_->state == STATE_READY);
		active_.info_->state = STATE_RUNNING;
		master_.switch_to_( active_);
		runnable_fibers_.pop_front();
		result = true;
	}
	else
		boost::move( active_);

	while ( ! terminated_fibers_.empty() )
	{
		fiber f( fibers_[terminated_fibers_.front()].f);
		terminated_fibers_.pop();
		BOOST_ASSERT( STATE_TERMINATED == f.info_->state);	
		BOOST_ASSERT( fibers_[f.get_id()].waiting.empty() );	
		fibers_.erase( f.get_id() );
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
