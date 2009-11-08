
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fiber/fiber.hpp>

#include <boost/assert.hpp>

#include <boost/fiber/scheduler.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fiber {

void
fiber::convert_thread_to_fiber()
{ detail::fiber_info_base::convert_thread_to_fiber(); }

fiber::fiber() :
	info_( new detail::fiber_info_default() )
{}

void
fiber::swap( fiber & other)
{ info_.swap( other.info_); }

fiber::id
fiber::get_id() const
{ return fiber::id( info_); }

bool
fiber::operator==( fiber const& other) const
{ return get_id() == other.get_id(); }

bool
fiber::operator!=( fiber const& other) const
{ return !( get_id() == other.get_id() ); }

void trampoline( fiber * self)
{
	BOOST_ASSERT( self);
	BOOST_ASSERT( self->info_);
	self->info_->run();
 	scheduler::exit();
}

}}

#include <boost/config/abi_suffix.hpp>
