
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fiber/fiber.hpp>

#include <boost/assert.hpp>

#include <boost/fiber/detail/fiber_state.hpp>
#include <boost/fiber/exceptions.hpp>
#include <boost/fiber/scheduler.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fibers {

#ifdef BOOST_HAS_RVALUE_REFS
detail::fiber_info_base::ptr_t
fiber::make_info_( std::size_t stack_size, void ( * fn)() )
{
	return detail::fiber_info_base::ptr_t(
		new detail::fiber_info< void( *)() >( fn, stack_size) );
}
#endif

fiber::fiber() :
	info_()
{}

fiber::fiber( detail::fiber_info_base::ptr_t info) :
	info_( info)
{}

#ifdef BOOST_HAS_RVALUE_REFS
fiber::fiber( fiber && other)
{ info_.swap( other.info_); }

fiber &
fiber::operator=( fiber && other)
{
	info_ = other.info_;
	other.info_.reset();
	return * this;
}

fiber &&
fiber::move()
{ return static_cast< fiber && >( * this); }
#else
fiber::fiber( boost::detail::fiber_move_t< fiber > f)
{
	info_ = f->info_;
	f->info_.reset();
}

fiber &
fiber::operator=( boost::detail::fiber_move_t< fiber > f)
{
	fiber new_fiber( f);
	swap( new_fiber);
	return * this;
}

fiber::operator boost::detail::fiber_move_t< fiber >()
{ return move(); }

boost::detail::fiber_move_t< fiber >
fiber::move()
{
	boost::detail::fiber_move_t< fiber > f( * this);
	return f;
}
#endif
fiber::operator unspecified_bool_type() const
{ return info_; }

bool
fiber::operator!() const
{ return ! info_; }

bool
fiber::operator==( fiber const& other) const
{ return get_id() == other.get_id(); }

bool
fiber::operator!=( fiber const& other) const
{ return !( get_id() == other.get_id() ); }

void
fiber::swap( fiber & other)
{ info_.swap( other.info_); }

fiber::id
fiber::get_id() const
{ return fiber::id( info_); }

bool
fiber::is_alive() const
{
	if ( ! info_) throw fiber_moved();
	return ( info_->state & IS_ALIVE_BIT_MASK) != 0;
}

int
fiber::priority() const
{
	if ( ! info_) throw fiber_moved();
	return info_->priority;
}

void
fiber::priority( int prio)
{
	if ( ! info_) throw fiber_moved();
	info_->priority = prio;
	if ( is_alive() ) scheduler::reschedule( get_id() );
}

void
fiber::interrupt()
{
	if ( ! info_) throw fiber_moved();
	scheduler::interrupt( get_id() );
}

bool
fiber::interruption_requested() const
{
	if ( ! info_) throw fiber_moved();
	return ( info_->interrupt & detail::INTERRUPTION_ENABLED) != 0;
}

void
fiber::cancel()
{
	if ( ! info_) throw fiber_moved();
	scheduler::cancel( get_id() );
}

void
fiber::suspend()
{
	if ( ! info_) throw fiber_moved();
	scheduler::suspend( get_id() );
}

void
fiber::resume()
{
	if ( ! info_) throw fiber_moved();
	scheduler::resume( get_id() );
}

void
fiber::join()
{
	if ( ! info_) throw fiber_moved();
	scheduler::join( get_id() );
}

}}

#include <boost/config/abi_suffix.hpp>
