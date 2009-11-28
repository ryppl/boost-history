
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fiber/round_robin.hpp>

#include <memory>
#include <utility>

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

#include <boost/fiber/detail/move.hpp>
#include <boost/fiber/detail/fiber_state.hpp>
#include <boost/fiber/exceptions.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fibers {

round_robin::round_robin() :
	fibers_(),
	runnable_fibers_(),
	terminated_fibers_()
{}

round_robin::~round_robin()
{
	BOOST_FOREACH( container::value_type va, fibers_)
	{ detach( va.second.f); }
}

void
round_robin::add( fiber f)
{
	if ( ! f) throw fiber_moved();

	BOOST_ASSERT( ! is_master( f) );
	BOOST_ASSERT( in_state_not_started( f) );

	// set state to ready
	set_state_ready( f);

	// attach to this scheduler
	attach( f);

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

void
round_robin::yield( fiber::id const& id)
{
	BOOST_ASSERT( active_fiber->get_id() == id);
	BOOST_ASSERT( in_state_running( ( * active_fiber) ) );
	BOOST_ASSERT( ! fibers_[active_fiber->get_id()].waiting_on);

	// set state ready
	set_state_ready( ( * active_fiber) );

	// put fiber to runnable-queue
	runnable_fibers_.push_back( id);

	// switch to master-fiber
	switch_between( * active_fiber, master_fiber);
}

void
round_robin::cancel( fiber::id const& id)
{
	container::iterator i = fibers_.find( id);
	if ( i == fibers_.end() ) return;
	schedulable s( i->second);
	fiber f( s.f);
	BOOST_ASSERT( f);
	BOOST_ASSERT( ! is_master( f) );
	BOOST_ASSERT( ! in_state_not_started( f) );

	// nothing to do for al terminated fiber
	if ( in_state_terminated( f) ) return;

	// invoke each fiber waiting on this fiber
	BOOST_FOREACH( fiber::id id__, s.joining_fibers)
	{
		schedulable s__( fibers_[id__]);
		fiber f__( s__.f);
		BOOST_ASSERT( s__.waiting_on);
		BOOST_ASSERT( in_state_wait_for_join( f__) );

		// clear waiting-on
		fibers_[id__].waiting_on.reset();

		// put fiber on runnable-queue
		set_state_ready( f__);
		runnable_fibers_.push_back( id__);
	}
	// clear waiting-queue
	fibers_[id].joining_fibers.clear();

	// if fiber is ready remove it from the runnable-queue
	// and put it to terminated-queue
	if ( in_state_ready( f) )
	{
		set_state_terminated( f);
		runnable_fibers_.remove( id);
		terminated_fibers_.push( id);	
	}
	// if fiber is running (== active fiber)
	// reset active fiber
	// put it to terminated-queue and switch
	// to master fiber
	else if ( in_state_running( f) )
	{
		BOOST_ASSERT( active_fiber->get_id() == id);
		set_state_terminated( f);
		terminated_fibers_.push( id);
		switch_between( f, master_fiber);
	}
	// if fiber is waiting then remove it from the
	// waiting-queue and put it to terminated-queue
	else if ( in_state_wait_for_join( f) )
	{
		BOOST_ASSERT( s.waiting_on);
		set_state_terminated( f);
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
	BOOST_ASSERT( ! is_master( f) );
	BOOST_ASSERT( ! in_state_not_started( f) );

	// nothing to do for a terminated fiber
	if ( in_state_terminated( f) ) return;

	// prevent self-join
	if ( active_fiber->get_id() == id)
		throw scheduler_error("self-join denied");

	// register on fiber to be joined
	fibers_[id].joining_fibers.push_back( active_fiber->get_id() );

	// set state waiting
	set_state_wait_for_join( ( * active_fiber) );

	// set fiber-id waiting-on
	fibers_[active_fiber->get_id()].waiting_on = id;

	// switch to master-fiber
	switch_between( * active_fiber, master_fiber);

	// fiber returned
	BOOST_ASSERT( in_state_running( ( * active_fiber) ) );
	BOOST_ASSERT( ! fibers_[active_fiber->get_id()].waiting_on);

	// check if interruption was requested
	if ( interruption_enabled( * active_fiber) )
		throw fiber_interrupted();
}

void
round_robin::interrupt( fiber::id const& id)
{
	container::iterator i = fibers_.find( id);
	if ( i == fibers_.end() ) return;
	schedulable s( i->second);
	fiber f( s.f);
	BOOST_ASSERT( f);
	BOOST_ASSERT( ! is_master( f) );
	BOOST_ASSERT( ! in_state_not_started( f) );

	// nothing to do for al terminated fiber
	if ( in_state_terminated( f) ) return;

	enable_interruption( f);

	// if fiber is waiting
	if ( in_state_wait_for_join( f) )
	{
		// fiber is waiting (joining) on another fiber
		// remove it from the waiting-queue, reset waiting-on
		// and reset the waiting state
		BOOST_ASSERT( s.waiting_on);
		fibers_[* s.waiting_on].joining_fibers.remove( id);
		fibers_[id].waiting_on.reset();
		set_state_ready( f);
		runnable_fibers_.push_back( id);
	}
}	

void
round_robin::reschedule( fiber::id const& id)
{
	container::iterator i = fibers_.find( id);
	if ( i == fibers_.end() ) return;
	fiber f( i->second.f);
	BOOST_ASSERT( f);
	BOOST_ASSERT( ! is_master( f) );
	BOOST_ASSERT( ! in_state_not_started( f) );
	BOOST_ASSERT( ! in_state_terminated( f) );
}

bool
round_robin::run()
{
	bool result( false);
	if ( ! runnable_fibers_.empty() )
	{
		schedulable s = fibers_[runnable_fibers_.front()];
		std::auto_ptr< fiber > orig( active_fiber.release() );
		active_fiber.reset( new fiber( s.f) );
		BOOST_ASSERT( ! s.waiting_on);
		BOOST_ASSERT( in_state_ready( ( * active_fiber) ) );
		set_state_running( ( * active_fiber) );
		switch_between( master_fiber, * active_fiber);
		active_fiber.reset( orig.release() );
		runnable_fibers_.pop_front();
		result = true;
	}

	while ( ! terminated_fibers_.empty() )
	{
		schedulable s = fibers_[terminated_fibers_.front()];
		fiber f( s.f);
		terminated_fibers_.pop();
		BOOST_ASSERT( s.joining_fibers.empty() );	
		BOOST_ASSERT( in_state_terminated( f) );	
		fibers_.erase( f.get_id() );
		result = true;
	}
	return result;
}

bool
round_robin::empty()
{ return fibers_.empty(); }

std::size_t
round_robin::size()
{ return fibers_.size(); }

}}

#include <boost/config/abi_suffix.hpp>
