
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_THIS_FIBER_UTILITY_H
#define BOOST_THIS_FIBER_UTILITY_H

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <boost/fiber/fiber.hpp>
#include <boost/fiber/strategy.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace this_fiber {

inline
bool runs_as_fiber()
{ return fibers::strategy::runs_as_fiber(); }

inline
fiber::id get_id()
{ return fibers::strategy::get_id(); }

inline
int priority()
{ return fibers::strategy::priority(); }

inline
void priority( int prio)
{ fibers::strategy::priority( prio); }

inline
void interruption_point()
{ fibers::strategy::interruption_point(); }

inline
bool interruption_requested()
{ return fibers::strategy::interruption_requested(); }

inline
bool interruption_enabled()
{ return fibers::strategy::interruption_enabled(); }

inline
void at_fiber_exit( function< void() > ca)
{ fibers::strategy::at_fiber_exit( ca); }

template< typename Callable >
void at_fiber_exit( Callable ca)
{ fibers::strategy::at_fiber_exit( boost::bind( boost::type< void >(), ca) ); }

inline
void at_fiber_exit( void ( * ca)() )
{ fibers::strategy::at_fiber_exit( boost::bind( boost::type< void >(), ca) ); }

inline
void yield()
{ fibers::strategy::yield(); }

inline
void cancel()
{ fibers::strategy::cancel(); }

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_THIS_FIBER_UTILITY_H
