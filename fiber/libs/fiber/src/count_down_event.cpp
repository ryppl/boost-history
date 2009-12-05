
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/fiber/count_down_event.hpp"

#include <boost/fiber/detail/atomic.hpp>
#include <boost/fiber/mutex.hpp>
#include <boost/fiber/strategy.hpp>

namespace boost {
namespace fibers {

count_down_event::count_down_event( uint32_t initial) :
	initial_( initial),
	current_( initial_),
	id_( * this)
{ strategy::register_object_( id_); }

count_down_event::~count_down_event()
{ strategy::unregister_object_( id_); }

uint32_t
count_down_event::initial() const
{ return initial_; }

uint32_t
count_down_event::current() const
{ return detail::atomic_load( & current_); }

bool
count_down_event::is_set() const
{ return 0 == detail::atomic_load( & current_); }

void
count_down_event::set()
{
	for (;;)
	{
		if ( 0 == detail::atomic_load( & current_) )
			return;
		uint32_t expected = current_;
		if ( detail::atomic_compare_exchange_strong( & current_, & expected, expected - 1) )
			break;
	}
	if ( 0 == detail::atomic_load( & current_) )
		strategy::object_notify_all_( id_);
}

void
count_down_event::wait()
{
	while ( 0 != detail::atomic_load( & current_) )
		strategy::wait_for_object_( id_);
}

}}
