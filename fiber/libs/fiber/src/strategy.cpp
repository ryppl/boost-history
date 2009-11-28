#include <boost/fiber/strategy.hpp>

#include <boost/assert.hpp>

#include <boost/fiber/detail/fiber_info.hpp>
#include <boost/fiber/detail/fiber_info_base.hpp>
#include <boost/fiber/exceptions.hpp>

namespace boost {
namespace fibers {

strategy::active_fiber_t strategy::active_fiber;

bool
strategy::runs_as_fiber()
{ return active_fiber.get() != 0; }

fiber::id
strategy::get_id()
{
	fiber * active( active_fiber.get() );
	if ( ! active) throw fiber_error("not a fiber");
	return active->get_id();
}

void
strategy::interruption_point()
{
	fiber * active( active_fiber.get() );
	if ( ! active) throw fiber_error("not a fiber");
	if ( active->info_()->interrupt == detail::INTERRUPTION_ENABLED)
		throw fiber_interrupted();
}

bool
strategy::interruption_requested()
{
	fiber * active( active_fiber.get() );
	if ( ! active) throw fiber_error("not a fiber");
	return active->interruption_requested();
}

detail::fiber_interrupt_t &
strategy::interrupt_flags()
{
	fiber * active( active_fiber.get() );
	if ( ! active) throw fiber_error("not a fiber");
	return active->info_()->interrupt;
}

bool
strategy::interruption_enabled()
{
	fiber * active( active_fiber.get() );
	if ( ! active) throw fiber_error("not a fiber");
	return active->info_()->interrupt & detail::INTERRUPTION_ENABLED;
}

int
strategy::priority()
{
	fiber * active( active_fiber.get() );
	if ( ! active) throw fiber_error("not a fiber");
	return active->priority();
}

void
strategy::priority( int prio)
{
	fiber * active( active_fiber.get() );
	if ( ! active) throw fiber_error("not a fiber");
	active->priority( prio);
}

void
strategy::at_fiber_exit( callable_t ca)
{
	fiber * active( active_fiber.get() );
	if ( ! active) throw fiber_error("not a fiber");
	active->info_()->at_exit.push( ca);
}

void
strategy::yield()
{
	fiber * active( active_fiber.get() );
	if ( ! active) throw fiber_error("not a fiber");
	active->info_()->st->yield( active->get_id() );
}

void
strategy::cancel()
{
	fiber * active( active_fiber.get() );
	if ( ! active) throw fiber_error("not a fiber");
	active->info_()->st->cancel( active->get_id() );
}

strategy::strategy() :
	master_fiber()
{
	fiber::convert_thread_to_fiber();
	master_fiber = fiber( 
		detail::fiber_info_base::ptr_t(
			new detail::fiber_info_default() ) );
}

strategy::~strategy()
{ fiber::convert_fiber_to_thread(); }

void
strategy::attach( fiber & f)
{ f.info_()->st = this; }

void
strategy::switch_between( fiber & from, fiber & to)
{ from.switch_to_( to); }

void
strategy::enable_interruption( fiber & f)
{
	// remove disabled flag
	f.info_()->interrupt &= ~detail::INTERRUPTION_DISABLED;

	// set enabled flag
	f.info_()->interrupt |= detail::INTERRUPTION_ENABLED;
}

bool
strategy::interruption_enabled( fiber const& f)
{ return detail::INTERRUPTION_ENABLED == f.info_()->interrupt; }

bool
strategy::is_master( fiber const& f)
{ return master_fiber == f; }

bool
strategy::in_state_not_started( fiber const& f)
{ return detail::STATE_NOT_STARTED == f.info_()->state; }

bool
strategy::in_state_ready( fiber const& f)
{ return detail::STATE_READY == f.info_()->state; }

bool
strategy::in_state_running( fiber const& f)
{ return detail::STATE_RUNNING == f.info_()->state; }

bool
strategy::in_state_wait_for_join( fiber const& f)
{ return detail::STATE_WAIT_FOR_JOIN == f.info_()->state; }

bool
strategy::in_state_terminated( fiber const& f)
{ return detail::STATE_TERMINATED == f.info_()->state; }

void
strategy::set_state_ready( fiber & f)
{ f.info_()->state = detail::STATE_READY; }

void
strategy::set_state_running( fiber & f)
{ f.info_()->state = detail::STATE_RUNNING; }

void
strategy::set_state_wait_for_join( fiber & f)
{ f.info_()->state = detail::STATE_WAIT_FOR_JOIN; }

void
strategy::set_state_terminated( fiber & f)
{ f.info_()->state = detail::STATE_TERMINATED; }

}}
