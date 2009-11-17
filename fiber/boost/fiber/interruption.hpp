
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_INTERRUPTION_H
#define BOOST_FIBERS_INTERRUPTION_H

#include <cstddef>

#include <boost/utility.hpp>

#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/fiber_state.hpp>
#include <boost/fiber/scheduler.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fibers {

class BOOST_FIBER_DECL disable_interruption : private noncopyable
{
private:
	bool	set_;

public:
	disable_interruption() :
		set_( ( scheduler::interrupt_flags_active_fiber() & detail::INTERRUPTION_BLOCKED) != 0)
	{
		if ( ! set_)
			scheduler::interrupt_flags_active_fiber() |= detail::INTERRUPTION_BLOCKED;
	}

	~disable_interruption()
	{
		if ( ! set_)
			scheduler::interrupt_flags_active_fiber() &= ~detail::INTERRUPTION_BLOCKED;
	}
};

class BOOST_FIBER_DECL restore_interruption : private noncopyable
{
private:
	bool	set_;

public:
	restore_interruption() :
		set_( ( scheduler::interrupt_flags_active_fiber() & detail::INTERRUPTION_BLOCKED) != 0)
	{
		if ( set_)
			scheduler::interrupt_flags_active_fiber() &= ~detail::INTERRUPTION_BLOCKED;
	}

	~restore_interruption()
	{  
		if ( set_)
			scheduler::interrupt_flags_active_fiber() |= detail::INTERRUPTION_BLOCKED;
	}
};

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_INTERRUPTION_H
