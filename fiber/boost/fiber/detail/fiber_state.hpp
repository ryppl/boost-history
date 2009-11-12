
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

enum fiber_state_t
{
	STATE_MASTER			= 0X00,
	STATE_NOT_STARTED		= 0X10,
	STATE_RUNNABLE			= 0x20,
		STATE_READY		= 0x21,
		STATE_RUNNING		= 0x22,
	STATE_NOT_RUNNABLE		= 0x40,
		STATE_SUSPENDED		= 0x41,
		STATE_WAITING		= 0x42,
	STATE_TERMINATED		= 0x80
};

}}

}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_DETAIL_FIBER_STATE_H
