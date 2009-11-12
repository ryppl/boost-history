
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_THIS_FIBER_UTILITY_H
#define BOOST_THIS_FIBER_UTILITY_H

#include <boost/config.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/thread/thread_time.hpp>

#include <boost/fiber/fiber.hpp>
#include <boost/fiber/scheduler.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace this_fiber {

inline
bool runs_as_fiber()
{ return fibers::scheduler::runs_as_fiber(); }

inline
fiber::id get_id()
{ return fibers::scheduler::get_id(); }

inline
void yield()
{ fibers::scheduler::yield_active_fiber(); }

inline
void cancel()
{ fibers::scheduler::terminate_active_fiber(); }

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_THIS_FIBER_UTILITY_H
