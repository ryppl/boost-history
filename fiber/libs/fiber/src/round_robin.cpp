
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fiber/detail/round_robin.hpp>

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

#define HAS_STATE_WAIT_FOR_JOIN( state) \
	( state & STATE_WAIT_FOR_JOIN) != 0

#define HAS_STATE_TERMINATED( state) \
	( state & STATE_TERMINATED) != 0

round_robin::round_robin() :
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

round_robin::~round_robin()
{ fiber::convert_fiber_to_thread(); }

void
round_robin::add( fiber const& f)
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
round_robin::get_id() const
{ return active_.get_id(); }

void
round_robin::yield()
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
round_robin::cancel()
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

		// if fiber is in state ready or running
		// put it on runnable-queue
		if ( ( HAS_STATE_READY( f__.info_()->state) || HAS_STATE_RUNNING( f__.info_()->state) ) )
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
round_robin::interrupt()
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
round_robin::interruption_requested() const
{
	BOOST_ASSERT( STATE_RUNNING == active_.info_()->state);

	return active_.interruption_requested();
}

bool
round_robin::interruption_enabled() const
{
	BOOST_ASSERT( STATE_RUNNING == active_.info_()->state);

	return active_.info_()->interrupt == detail::INTERRUPTION_ENABLED;
}

fiber_interrupt_t &
round_robin::interrupt_flags()
{
	BOOST_ASSERT( STATE_RUNNING == active_.info_()->state);
	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	return active_.info_()->interrupt;
}

int
round_robin::priority() const
{
	BOOST_ASSERT( STATE_RUNNING == active_.info_()->state);

	return active_.priority();
}

void
round_robin::priority( int prio)
{
	BOOST_ASSERT( STATE_RUNNING == active_.info_()->state);
	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	// set priority
	active_.priority( prio);
}

void
round_robin::at_exit( callable_t ca)
{
	BOOST_ASSERT( STATE_RUNNING == active_.info_()->state);
	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	// push a exit-callback on fibers stack
	active_.info_()->at_exit.push( ca);
}

void
round_robin::interrupt( fiber::id const& id)
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

		BOOST_ASSERT(
				HAS_STATE_READY( f.info_()->state) ||
				HAS_STATE_RUNNING( f.info_()->state) );
		f.info_()->state = STATE_READY;
		runnable_fibers_.push_back( id);
	}
}	

void
round_robin::cancel( fiber::id const& id)
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

		// if fiber is in state ready or running
		// put it on runnable-queue
		if ( ( HAS_STATE_READY( f__.info_()->state) || HAS_STATE_RUNNING( f__.info_()->state) ) )
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
	else
		BOOST_ASSERT( ! "should never reached");
}

void
round_robin::join( fiber::id const& id)
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
round_robin::reschedule( fiber::id const& id)
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
round_robin::run()
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
round_robin::empty() const
{ return fibers_.empty(); }

std::size_t
round_robin::size() const
{ return fibers_.size(); }

#undef HAS_STATE_MASTER
#undef HAS_STATE_NOT_STARTED
#undef HAS_STATE_READY
#undef HAS_STATE_RUNNING
#undef HAS_STATE_WAIT_FOR_JOIN
#undef HAS_STATE_TERMINATED

}}}

#include <boost/config/abi_suffix.hpp>
