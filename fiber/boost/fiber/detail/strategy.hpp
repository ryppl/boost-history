
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_DETAIL_STRATEGY_H
#define BOOST_FIBERS_DETAIL_STRATEGY_H

#include <cstddef>

#include <boost/function.hpp>

#include <boost/fiber/fiber.hpp>

#include <boost/config/abi_prefix.hpp>

# if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable:4251 4275)
# endif

namespace boost {
namespace fibers {
namespace detail {

struct BOOST_FIBER_DECL strategy
{
	typedef function< void() >	callable_t;

	virtual ~strategy() {}

	virtual void add( fiber const&) = 0;

	virtual fiber::id get_id() const = 0;

	virtual void yield() = 0;

	virtual void cancel() = 0;

	virtual void interrupt() = 0;

	virtual bool interruption_requested() const = 0;

	virtual bool interruption_enabled() const = 0;

	virtual fiber_interrupt_t & interrupt_flags() = 0;

	virtual int priority() const = 0;

	virtual void priority( int) = 0;

	virtual void at_exit( callable_t) = 0;

	virtual void interrupt( fiber::id const&) = 0;

	virtual void cancel( fiber::id const&) = 0;

	virtual void join( fiber::id const&) = 0;

	virtual void reschedule( fiber::id const&) = 0;

	virtual bool run() = 0;

	virtual bool empty() const = 0;

	virtual std::size_t size() const = 0;
};

}}}

# if defined(BOOST_MSVC)
# pragma warning(pop)
# endif

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_DETAIL_STRATEGY_H
