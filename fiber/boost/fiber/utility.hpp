
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_THIS_FIBER_UTILITY_H
#define BOOST_THIS_FIBER_UTILITY_H

#include <boost/bind.hpp>
#include <boost/config.hpp>
#include <boost/function.hpp>

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
{ fibers::scheduler::yield(); }

inline
void cancel()
{ fibers::scheduler::cancel(); }

inline
int priority()
{ return fibers::scheduler::priority(); }

inline
void priority( int prio)
{ fibers::scheduler::priority( prio); }

inline
void interruption_point()
{ fibers::scheduler::interrupt(); }

inline
bool interruption_requested()
{ return fibers::scheduler::interruption_requested(); }

inline
bool interruption_enabled()
{ return fibers::scheduler::interruption_enabled(); }

template< typename Callable >
void at_fiber_exit( Callable ca)
{
	fibers::scheduler::at_exit(
		boost::bind( boost::type< void >(), ca) );
}

inline
void at_fiber_exit( function< void() > ca)
{ fibers::scheduler::at_exit( ca); }

inline
void at_fiber_exit( void ( * ca)() )
{
	fibers::scheduler::at_exit(
		boost::bind( boost::type< void >(), ca) );
}

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_THIS_FIBER_UTILITY_H
