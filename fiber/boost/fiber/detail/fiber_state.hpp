
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_DETAIL_FIBER_STATE_H
#define BOOST_FIBERS_DETAIL_FIBER_STATE_H

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fibers {
namespace detail {

enum state_t
{
	STATE_MASTER		= 1 << 0,
	STATE_NOT_STARTED	= 1 << 1,
	STATE_READY			= 1 << 2,
	STATE_RUNNING		= 1 << 3,
	STATE_SUSPENDED		= 1 << 4,
	STATE_WAIT_FOR_JOIN	= 1 << 5,
	STATE_TERMINATED	= 1 << 6
};

typedef char fiber_state_t;

#define IS_ALIVE_BIT_MASK 0x3C

enum interrupt_t
{
	INTERRUPTION_DISABLED	= 1 << 0,
	INTERRUPTION_ENABLED	= 1 << 1,
	INTERRUPTION_BLOCKED	= 1 << 2
};

typedef char fiber_interrupt_t;

}}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_DETAIL_FIBER_STATE_H
