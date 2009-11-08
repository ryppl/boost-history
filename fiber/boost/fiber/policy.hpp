
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBER_POLICY_H
#define BOOST_FIBER_POLICY_H

#include <cstddef>
#include <memory>

#include <boost/fiber/fiber.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fiber {

struct policy
{
	virtual ~policy() {}

	virtual void add_fiber( std::auto_ptr< fiber >) = 0;

	virtual fiber::id active_fiber() = 0;

	virtual void yield_active_fiber() = 0;

	virtual void exit_active_fiber() = 0;

	virtual bool run() = 0;

	virtual bool empty() = 0;

	virtual std::size_t size() = 0;
};

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBER_POLICY_H
