
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_THIS_FIBER_UTILITY_H
#define BOOST_THIS_FIBER_UTILITY_H

#include <boost/config.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/thread/thread_time.hpp>

#include <boost/fiber/detail/fiber.hpp>
#include <boost/fiber/scheduler.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace this_fiber {

inline
fiber::id get_id()
{ return fiber::scheduler::get_id(); }

inline
void yield()
{ fiber::scheduler::yield(); }

inline
void sleep( system_time const& abs_time)
{}

template< typename TimeDuration >
void sleep( TimeDuration const& rel_time)
{}

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_THIS_FIBER_UTILITY_H