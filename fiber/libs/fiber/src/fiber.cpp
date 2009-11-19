
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
	info_base_()
{}

fiber::fiber( detail::fiber_info_base::ptr_t info_base) :
	info_base_( info_base)
{}

#ifdef BOOST_HAS_RVALUE_REFS
fiber::fiber( fiber && other)
{ info_base_.swap( other.info_base_); }

fiber &
fiber::operator=( fiber && other)
{
	info_base_ = other.info_base_;
	other.info_base_.reset();
	return * this;
}

fiber &&
fiber::move()
{ return static_cast< fiber && >( * this); }
#else
fiber::fiber( boost::detail::fiber_move_t< fiber > f)
{
	info_base_ = f->info_base_;
	f->info_base_.reset();
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

detail::fiber_info_base::ptr_t
fiber::info_() const
{ return info_base_; }

fiber::operator unspecified_bool_type() const
{ return info_base_; }

bool
fiber::operator!() const
{ return ! info_base_; }

bool
fiber::operator==( fiber const& other) const
{ return get_id() == other.get_id(); }

bool
fiber::operator!=( fiber const& other) const
{ return !( get_id() == other.get_id() ); }

void
fiber::swap( fiber & other)
{ info_base_.swap( other.info_base_); }

fiber::id
fiber::get_id() const
{ return fiber::id( info_base_); }

bool
fiber::is_alive() const
{
	if ( ! info_base_) throw fiber_moved();
	return ( info_base_->state & IS_ALIVE_BIT_MASK) != 0;
}

int
fiber::priority() const
{
	if ( ! info_base_) throw fiber_moved();
	return info_base_->priority;
}

void
fiber::priority( int prio)
{
	if ( ! info_base_) throw fiber_moved();
	info_base_->priority = prio;
	if ( is_alive() ) scheduler::reschedule( get_id() );
}

void
fiber::interrupt()
{
	if ( ! info_base_) throw fiber_moved();
	scheduler::interrupt( get_id() );
}

bool
fiber::interruption_requested() const
{
	if ( ! info_base_) throw fiber_moved();
	return ( info_base_->interrupt & detail::INTERRUPTION_ENABLED) != 0;
}

void
fiber::cancel()
{
	if ( ! info_base_) throw fiber_moved();
	scheduler::cancel( get_id() );
}

void
fiber::suspend()
{
	if ( ! info_base_) throw fiber_moved();
	scheduler::suspend( get_id() );
}

void
fiber::resume()
{
	if ( ! info_base_) throw fiber_moved();
	scheduler::resume( get_id() );
}

void
fiber::join()
{
	if ( ! info_base_) throw fiber_moved();
	scheduler::join( get_id() );
}

}}

#include <boost/config/abi_suffix.hpp>
