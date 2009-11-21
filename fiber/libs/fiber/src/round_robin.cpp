
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

#define SET_STATE_NOT_STARTED( f) \
	f.info_()->state = STATE_NOT_STARTED;

#define SET_STATE_READY( f) \
	f.info_()->state = STATE_READY;

#define SET_STATE_RUNNING( f) \
	f.info_()->state = STATE_RUNNING;

#define SET_STATE_WAIT_FOR_JOIN( f) \
	f.info_()->state = STATE_WAIT_FOR_JOIN;

#define SET_STATE_TERMINATED( f) \
	f.info_()->state = STATE_TERMINATED

#define IN_STATE_MASTER( f) \
	( f.info_()->state == STATE_MASTER)

#define IN_STATE_NOT_STARTED( f) \
	( f.info_()->state == STATE_NOT_STARTED)

#define IN_STATE_READY( f) \
	( f.info_()->state == STATE_READY)

#define IN_STATE_RUNNING( f) \
	( f.info_()->state == STATE_RUNNING)

#define IN_STATE_WAIT_FOR_JOIN( f) \
	( f.info_()->state == STATE_WAIT_FOR_JOIN)

#define IN_STATE_TERMINATED( f) \
	( f.info_()->state == STATE_TERMINATED)

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

	BOOST_ASSERT( ! IN_STATE_MASTER( f) );
	BOOST_ASSERT( IN_STATE_NOT_STARTED( f) );

	// set state to ready
	SET_STATE_READY( f);

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
{
	BOOST_ASSERT( IN_STATE_RUNNING( active_) );
//	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	return active_.get_id();
}

void
round_robin::yield()
{
	BOOST_ASSERT( IN_STATE_RUNNING( active_) );
	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	// set state ready
	SET_STATE_READY( active_);

	// put fiber to runnable-queue
	runnable_fibers_.push_back( active_.get_id() );

	// switch to master-fiber
	active_.switch_to_( master_);
}

void
round_robin::cancel()
{
	BOOST_ASSERT( IN_STATE_RUNNING( active_) );
	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	schedulable s( fibers_[active_.get_id()]);
	// invoke each fiber waiting on this fiber
	BOOST_FOREACH( fiber::id id__, s.joining_fibers)
	{
		schedulable s__( fibers_[id__]);
		fiber f__( s__.f);
		BOOST_ASSERT( s__.waiting_on);
		BOOST_ASSERT( active_.get_id() == * s__.waiting_on);
		BOOST_ASSERT( IN_STATE_WAIT_FOR_JOIN( f__) );

		// clear waiting-on
		fibers_[id__].waiting_on.reset();

		// put fiber on runnable-queue
		SET_STATE_READY( f__);
		runnable_fibers_.push_back( id__);
	}
	// clear waiting-queue
	fibers_[active_.get_id()].joining_fibers.clear();

	// set state to terminated
	SET_STATE_TERMINATED( active_);

	// put fiber to terminated-queue
	terminated_fibers_.push( active_.get_id() );

	// switch to master-fiber
	active_.switch_to_( master_);
}

void
round_robin::interrupt()
{
	BOOST_ASSERT( IN_STATE_RUNNING( active_) );
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
	BOOST_ASSERT( IN_STATE_RUNNING( active_) );
//	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	return active_.interruption_requested();
}

bool
round_robin::interruption_enabled() const
{
	BOOST_ASSERT( IN_STATE_RUNNING( active_) );
//	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	return active_.info_()->interrupt == detail::INTERRUPTION_ENABLED;
}

fiber_interrupt_t &
round_robin::interrupt_flags()
{
	BOOST_ASSERT( IN_STATE_RUNNING( active_) );
	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	return active_.info_()->interrupt;
}

int
round_robin::priority() const
{
	BOOST_ASSERT( IN_STATE_RUNNING( active_) );
//	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	return active_.priority();
}

void
round_robin::priority( int prio)
{
	BOOST_ASSERT( IN_STATE_RUNNING( active_) );
	BOOST_ASSERT( ! fibers_[active_.get_id()].waiting_on);

	// set priority
	active_.priority( prio);
}

void
round_robin::at_exit( callable_t ca)
{
	BOOST_ASSERT( IN_STATE_RUNNING( active_) );
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
	BOOST_ASSERT( ! IN_STATE_MASTER( f) );
	BOOST_ASSERT( ! IN_STATE_NOT_STARTED( f) );

	// nothing to do for al terminated fiber
	if ( IN_STATE_TERMINATED( f) ) return;

	// remove disabled flag
	f.info_()->interrupt &= ~detail::INTERRUPTION_DISABLED;

	// set enabled flag
	f.info_()->interrupt |= detail::INTERRUPTION_ENABLED;

	// if fiber is waiting
	if ( IN_STATE_WAIT_FOR_JOIN( f) )
	{
		// fiber is waiting (joining) on another fiber
		// remove it from the waiting-queue, reset waiting-on
		// and reset the waiting state
		BOOST_ASSERT( s.waiting_on);
		fibers_[* s.waiting_on].joining_fibers.remove( id);
		fibers_[id].waiting_on.reset();
		SET_STATE_READY( f);
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
	BOOST_ASSERT( ! IN_STATE_MASTER( f) );
	BOOST_ASSERT( ! IN_STATE_NOT_STARTED( f) );

	// nothing to do for al terminated fiber
	if ( IN_STATE_TERMINATED( f) ) return;

	// invoke each fiber waiting on this fiber
	BOOST_FOREACH( fiber::id id__, s.joining_fibers)
	{
		schedulable s__( fibers_[id__]);
		fiber f__( s__.f);
		BOOST_ASSERT( s__.waiting_on);
		BOOST_ASSERT( IN_STATE_WAIT_FOR_JOIN( f__) );

		// clear waiting-on
		fibers_[id__].waiting_on.reset();

		// put fiber on runnable-queue
		SET_STATE_READY( f__);
		runnable_fibers_.push_back( id__);
	}
	// clear waiting-queue
	fibers_[id].joining_fibers.clear();

	// if fiber is ready remove it from the runnable-queue
	// and put it to terminated-queue
	if ( IN_STATE_READY( f) )
	{
		SET_STATE_TERMINATED( f);
		runnable_fibers_.remove( id);
		terminated_fibers_.push( id);	
	}
	// if fiber is running (== active fiber)
	// put it to terminated-queue and switch
	// to master fiber
	else if ( IN_STATE_RUNNING( f) )
	{
		BOOST_ASSERT( active_.get_id() == id);
		SET_STATE_TERMINATED( f);
		terminated_fibers_.push( id);
		f.switch_to_( master_);
	}
	// if fiber is waiting then remove it from the
	// waiting-queue and put it to terminated-queue
	else if ( IN_STATE_WAIT_FOR_JOIN( f) )
	{
		BOOST_ASSERT( s.waiting_on);
		SET_STATE_TERMINATED( f);
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
	BOOST_ASSERT( ! IN_STATE_MASTER( f) );
	BOOST_ASSERT( ! IN_STATE_NOT_STARTED( f) );

	// nothing to do for a terminated fiber
	if ( IN_STATE_TERMINATED( f) ) return;

	// prevent self-join
	if ( active_.get_id() == id) throw scheduler_error("self-join denied");

	// register on fiber to be joined
	fibers_[id].joining_fibers.push_back( active_.get_id() );

	// set state waiting
	SET_STATE_WAIT_FOR_JOIN( active_);

	// set fiber-id waiting-on
	fibers_[active_.get_id()].waiting_on = id;

	// switch to master-fiber
	active_.switch_to_( master_);

	// fiber returned
	BOOST_ASSERT( IN_STATE_RUNNING( active_) );
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
	BOOST_ASSERT( ! IN_STATE_MASTER( f) );
	BOOST_ASSERT( ! IN_STATE_NOT_STARTED( f) );
	BOOST_ASSERT( ! IN_STATE_TERMINATED( f) );

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
		BOOST_ASSERT( IN_STATE_READY( active_) );
		SET_STATE_RUNNING( active_);
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
		BOOST_ASSERT( IN_STATE_TERMINATED( f) );	
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

#undef IN_STATE_MASTER
#undef IN_STATE_NOT_STARTED
#undef IN_STATE_READY
#undef IN_STATE_RUNNING
#undef IN_STATE_WAIT_FOR_JOIN
#undef IN_STATE_TERMINATED
#undef SET_STATE_NOT_STARTED
#undef SET_STATE_READY
#undef SET_STATE_RUNNING
#undef SET_STATE_WAIT_FOR_JOIN
#undef SET_STATE_TERMINATED

}}}

#include <boost/config/abi_suffix.hpp>
