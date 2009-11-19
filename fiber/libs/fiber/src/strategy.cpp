
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fiber/detail/strategy.hpp>

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

#define HAS_STATE_WAIT_FOR_JOIN( state) \
	( state & STATE_WAIT_FOR_JOIN) != 0

#define HAS_STATE_TERMINATED( state) \
	( state & STATE_TERMINATED) != 0

strategy::strategy() :
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

strategy::~strategy()
{ fiber::convert_fiber_to_thread(); }

void
strategy::add( fiber f)
{
	if ( ! f) throw fiber_moved();

	BOOST_ASSERT( ! HAS_STATE_MASTER( f.info_()->state) );
	BOOST_ASSERT( STATE_NOT_STARTED == f.info_()->state);

	// set state to ready
	f.info_()->state = STATE_READY;

	// insert fiber to fiber-list
	std::pair< std::map< fiber::id, schedulable >::iterator, bool > result(
		fibers_.insert(
			std::make_pair(
				f.get_id(),
				schedulable( f) ) ) );

	// check result
	if ( ! result.second) throw scheduler_error("inserting fiber failed");

	// put fiber to runnable-queue
	runnable_fibers_.push_back( result.first->first);
}

fiber::id
strategy::get_id() const
{ return active_.get_id(); }

void
strategy::yield()
{
	BOOST_ASSERT( STATE_RUNNING == active_.info_()->state);
	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	// set state ready
	active_.info_()->state = STATE_READY;

	// put fiber to runnable-queue
	runnable_fibers_.push_back( active_.get_id() );

	// switch to master-fiber
	active_.switch_to_( master_);
}

void
strategy::cancel()
{
	BOOST_ASSERT( STATE_RUNNING == active_.info_()->state);
	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	schedulable s( fibers_[active_.get_id()]);

	// invoke each fiber waiting on this fiber
	BOOST_FOREACH( fiber::id id__, s.joining_fibers)
	{
		schedulable s__( fibers_[id__]);
		fiber f__( s__.f);
		BOOST_ASSERT( s__.waiting_on);
		BOOST_ASSERT( active_.get_id() == * s__.waiting_on);
		BOOST_ASSERT( HAS_STATE_WAIT_FOR_JOIN( f__.info_()->state) );

		// clear waiting-on
		fibers_[id__].waiting_on.reset();

		// remove wait-for-join state
		f__.info_()->state &= ~STATE_WAIT_FOR_JOIN;

		// if fiber is in state ready or running and not suspended
		// put it on runnable-queue
		if ( ( HAS_STATE_READY( f__.info_()->state) || HAS_STATE_RUNNING( f__.info_()->state) )
		     && ! HAS_STATE_SUSPENDED( f__.info_()->state)  )
		{
			f__.info_()->state = STATE_READY;
			runnable_fibers_.push_back( id__);
		}
	}
	// clear waiting-queue
	fibers_[active_.get_id()].joining_fibers.clear();

	// set state to terminated
	active_.info_()->state = STATE_TERMINATED;

	// put fiber to terminated-queue
	terminated_fibers_.push( active_.get_id() );

	// switch to master-fiber
	active_.switch_to_( master_);
}

void
strategy::suspend()
{
	BOOST_ASSERT( STATE_RUNNING == active_.info_()->state);
	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	// set state suspended
	active_.info_()->state |= STATE_SUSPENDED;

	// switch to master-fiber
	active_.switch_to_( master_);
}

void
strategy::interrupt()
{
	BOOST_ASSERT( STATE_RUNNING == active_.info_()->state);
	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	// gets invoked by interruption-points
	// if interruption flag is set throw fiber_interrupted
	// exceptions
	if ( INTERRUPTION_ENABLED == active_.info_()->interrupt)
		throw fiber_interrupted();
}

bool
strategy::interruption_requested()
{
	BOOST_ASSERT( STATE_RUNNING == active_.info_()->state);
	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	return active_.interruption_requested();
}

bool
strategy::interruption_enabled()
{
	BOOST_ASSERT( STATE_RUNNING == active_.info_()->state);
	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	return active_.info_()->interrupt == detail::INTERRUPTION_ENABLED;
}

fiber_interrupt_t &
strategy::interrupt_flags()
{
	BOOST_ASSERT( STATE_RUNNING == active_.info_()->state);
	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	return active_.info_()->interrupt;
}

int
strategy::priority()
{
	BOOST_ASSERT( STATE_RUNNING == active_.info_()->state);
	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	return active_.priority();
}

void
strategy::priority( int prio)
{
	BOOST_ASSERT( STATE_RUNNING == active_.info_()->state);
	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	// set priority
	active_.priority( prio);
}

void
strategy::at_exit( callable_t ca)
{
	BOOST_ASSERT( STATE_RUNNING == active_.info_()->state);
	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	// push a exit-callback on fibers stack
	active_.info_()->at_exit.push( ca);
}

void
strategy::interrupt( fiber::id const& id)
{
	container::iterator i = fibers_.find( id);
	if ( i == fibers_.end() ) return;
	schedulable s( i->second);
	fiber f( s.f);
	BOOST_ASSERT( f);
	BOOST_ASSERT( ! HAS_STATE_MASTER( f.info_()->state) );
	BOOST_ASSERT( ! HAS_STATE_NOT_STARTED( f.info_()->state) );

	// nothing to do for al terminated fiber
	if ( HAS_STATE_TERMINATED( f.info_()->state) ) return;

	// remove disabled flag
	f.info_()->interrupt &= ~detail::INTERRUPTION_DISABLED;

	// set enabled flag
	f.info_()->interrupt |= detail::INTERRUPTION_ENABLED;

	// if fiber is waiting
	if ( HAS_STATE_WAIT_FOR_JOIN( f.info_()->state) )
	{
		// fiber is waiting (joining) on another fiber
		// remove it from the waiting-queue, reset waiting-on
		// and reset the waiting state
		BOOST_ASSERT( s.waiting_on);
		fibers_[* s.waiting_on].joining_fibers.remove( id);
		fibers_[id].waiting_on.reset();
		f.info_()->interrupt &= ~STATE_WAIT_FOR_JOIN;

		// if fiber is not suspended put it to runnable-queue
		if ( ! HAS_STATE_SUSPENDED( f.info_()->state) )
		{
			BOOST_ASSERT(
					HAS_STATE_READY( f.info_()->state) ||
					HAS_STATE_RUNNING( f.info_()->state) );
			f.info_()->state = STATE_READY;
			runnable_fibers_.push_back( id);
		}
	}
}	

void
strategy::cancel( fiber::id const& id)
{
	container::iterator i = fibers_.find( id);
	if ( i == fibers_.end() ) return;
	schedulable s( i->second);
	fiber f( s.f);
	BOOST_ASSERT( f);
	BOOST_ASSERT( ! HAS_STATE_MASTER( f.info_()->state) );
	BOOST_ASSERT( ! HAS_STATE_NOT_STARTED( f.info_()->state) );

	// nothing to do for al terminated fiber
	if ( HAS_STATE_TERMINATED( f.info_()->state) ) return;

	// invoke each fiber waiting on this fiber
	BOOST_FOREACH( fiber::id id__, s.joining_fibers)
	{
		schedulable s__( fibers_[id__]);
		fiber f__( s__.f);
		BOOST_ASSERT( s__.waiting_on);
		BOOST_ASSERT( HAS_STATE_WAIT_FOR_JOIN( f__.info_()->state) );

		// clear waiting-on
		fibers_[id__].waiting_on.reset();

		// remove wait-for-join state
		f__.info_()->state &= ~STATE_WAIT_FOR_JOIN;

		// if fiber is in state ready or running and not suspended
		// put it on runnable-queue
		if ( ( HAS_STATE_READY( f__.info_()->state) || HAS_STATE_RUNNING( f__.info_()->state) )
		     && ! HAS_STATE_SUSPENDED( f__.info_()->state)  )
		{
			f__.info_()->state = STATE_READY;
			runnable_fibers_.push_back( id__);
		}
	}
	// clear waiting-queue
	fibers_[id].joining_fibers.clear();

	// if fiber is ready remove it from the runnable-queue
	// and put it to terminated-queue
	if ( HAS_STATE_READY( f.info_()->state) )
	{
		f.info_()->state = STATE_TERMINATED;
		runnable_fibers_.remove( id);
		terminated_fibers_.push( id);	
	}
	// if fiber is running (== active fiber)
	// put it to terminated-queue and switch
	// to master fiber
	else if ( HAS_STATE_RUNNING( f.info_()->state) )
	{
		BOOST_ASSERT( active_.get_id() == id);
		f.info_()->state = STATE_TERMINATED;
		terminated_fibers_.push( id);
		f.switch_to_( master_);
	}
	// if fiber is waiting then remove it from the
	// waiting-queue and put it to terminated-queue
	else if ( HAS_STATE_WAIT_FOR_JOIN( f.info_()->state) )
	{
		BOOST_ASSERT( s.waiting_on);
		f.info_()->state = STATE_TERMINATED;
		fibers_[* s.waiting_on].joining_fibers.remove( id);
		terminated_fibers_.push( id);	
	}
	// suspended state is only used with one of the
	// other states
	else
		BOOST_ASSERT( ! "should never reached");
}

void
strategy::suspend( fiber::id const& id)
{
	container::iterator i = fibers_.find( id);
	if ( i == fibers_.end() ) return;
	fiber f( i->second.f);
	BOOST_ASSERT( f);
	BOOST_ASSERT( ! HAS_STATE_MASTER( f.info_()->state) );
	BOOST_ASSERT( ! HAS_STATE_NOT_STARTED( f.info_()->state) );

	// nothing to do for a terminated fiber
	if ( HAS_STATE_TERMINATED( f.info_()->state) ) return;

	// if fiber is ready remove it from the
	// runnable-queue
	if ( HAS_STATE_READY( f.info_()->state) )
	{
		f.info_()->state |= STATE_SUSPENDED;
		runnable_fibers_.remove( id);
	}
	// if fiber is running (== active fiber)
	// switch to master-fiber
	else if ( HAS_STATE_RUNNING( f.info_()->state) )
	{
		BOOST_ASSERT( active_.get_id() == id);
		f.info_()->state |= STATE_SUSPENDED;
		f.switch_to_( master_);
	}
	// if fiber is in waiting state mark it only
	// as suspended
	else if ( HAS_STATE_WAIT_FOR_JOIN( f.info_()->state) )
		f.info_()->state |= STATE_SUSPENDED;
	else
		BOOST_ASSERT( ! "should never reached");
}

void
strategy::resume( fiber::id const& id)
{
	container::iterator i = fibers_.find( id);
	if ( i == fibers_.end() ) return;
	fiber f( i->second.f);
	BOOST_ASSERT( f);
	BOOST_ASSERT( ! HAS_STATE_MASTER( f.info_()->state) );
	BOOST_ASSERT( ! HAS_STATE_NOT_STARTED( f.info_()->state) );

	BOOST_ASSERT( active_.get_id() != id);

	// nothing to do for already terminated fiber
	if ( HAS_STATE_TERMINATED( f.info_()->state) ) return;

	// remove suspended state an requeue fiber
	if ( HAS_STATE_SUSPENDED( f.info_()->state) )
	{
		f.info_()->state &= ~STATE_SUSPENDED;
		// if suspended fiber was ready or running and not waiting
		// put it to the runnable-queue
		if ( ( HAS_STATE_READY( f.info_()->state) || HAS_STATE_RUNNING( f.info_()->state) )
			 && ! HAS_STATE_WAIT_FOR_JOIN( f.info_()->state) )
		{
			f.info_()->state = STATE_READY;
			runnable_fibers_.push_back( id);
		}
	}
}

void
strategy::join( fiber::id const& id)
{
	container::iterator i = fibers_.find( id);
	if ( i == fibers_.end() ) return;
	schedulable s( i->second);
	fiber f( s.f);
	BOOST_ASSERT( f);
	BOOST_ASSERT( ! HAS_STATE_MASTER( f.info_()->state) );
	BOOST_ASSERT( ! HAS_STATE_NOT_STARTED( f.info_()->state) );

	// nothing to do for a terminated fiber
	if ( HAS_STATE_TERMINATED( f.info_()->state) ) return;

	// prevent self-join
	if ( active_.get_id() == id) throw scheduler_error("self-join denied");

	// register on fiber to be joined
	fibers_[id].joining_fibers.push_back( active_.get_id() );

	// set state waiting
	active_.info_()->state |= STATE_WAIT_FOR_JOIN;

	// set fiber-id waiting-on
	fibers_[active_.get_id()].waiting_on = id;

	// switch to master-fiber
	active_.switch_to_( master_);

	// fiber was invoked
	BOOST_ASSERT( ! HAS_STATE_WAIT_FOR_JOIN( active_.info_()->state) );
	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	// check if interruption was requested
	interrupt();
}

void
strategy::reschedule( fiber::id const& id)
{
	container::iterator i = fibers_.find( id);
	if ( i == fibers_.end() ) return;
	fiber f( i->second.f);
	BOOST_ASSERT( f);
	BOOST_ASSERT( ! HAS_STATE_MASTER( f.info_()->state) );
	BOOST_ASSERT( ! HAS_STATE_NOT_STARTED( f.info_()->state) );
	BOOST_ASSERT( ! HAS_STATE_TERMINATED( f.info_()->state) );

	// TODO: re-schedule fiber == remove from
	// runnable_fibers + re-insert
}

bool
strategy::run()
{
	bool result( false);
	if ( ! runnable_fibers_.empty() )
	{
		schedulable s = fibers_[runnable_fibers_.front()];
		active_ = s.f;
		BOOST_ASSERT( ! s.waiting_on);
		BOOST_ASSERT( active_.info_()->state == STATE_READY);
		active_.info_()->state = STATE_RUNNING;
		master_.switch_to_( active_);
		runnable_fibers_.pop_front();
		result = true;
	}
	else
		active_.move();

	while ( ! terminated_fibers_.empty() )
	{
		schedulable s = fibers_[terminated_fibers_.front()];
		fiber f( s.f);
		terminated_fibers_.pop();
		BOOST_ASSERT( s.joining_fibers.empty() );	
		BOOST_ASSERT( STATE_TERMINATED == f.info_()->state);	
		fibers_.erase( f.get_id() );
		result = true;
	}
	return result;
}

bool
strategy::empty()
{ return fibers_.empty(); }

std::size_t
strategy::size()
{ return fibers_.size(); }

#undef HAS_STATE_MASTER
#undef HAS_STATE_NOT_STARTED
#undef HAS_STATE_READY
#undef HAS_STATE_RUNNING
#undef HAS_STATE_SUSPENDED
#undef HAS_STATE_WAIT_FOR_JOIN
#undef HAS_STATE_TERMINATED

}}}

#include <boost/config/abi_suffix.hpp>
