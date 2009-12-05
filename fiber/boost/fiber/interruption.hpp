
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_THIS_FIBER_INTERRUPTION_H
#define BOOST_THIS_FIBER_INTERRUPTION_H

#include <cstddef>

#include <boost/utility.hpp>

#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/interrupt_flags.hpp>
#include <boost/fiber/strategy.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace this_fiber {

class restore_interruption;

class BOOST_FIBER_DECL disable_interruption : private noncopyable
{
private:
	friend class restore_interruption;

	bool	set_;

public:
	disable_interruption() :
		set_( ( fibers::strategy::interrupt_flags_() & fibers::detail::INTERRUPTION_BLOCKED) != 0)
	{
		if ( ! set_)
			fibers::strategy::interrupt_flags_() |= fibers::detail::INTERRUPTION_BLOCKED;
	}

	~disable_interruption()
	{
		try
		{
			if ( ! set_)
				fibers::strategy::interrupt_flags_() &= ~fibers::detail::INTERRUPTION_BLOCKED;
		}
		catch (...)
		{}
	}
};

class BOOST_FIBER_DECL restore_interruption : private noncopyable
{
private:
	disable_interruption	&	disabler_;

public:
	explicit restore_interruption( disable_interruption & disabler) :
		disabler_( disabler)
	{
		if ( ! disabler_.set_)
			fibers::strategy::interrupt_flags_() &= ~fibers::detail::INTERRUPTION_BLOCKED;
	}

	~restore_interruption()
	{
	   try
	   {	   
			if ( ! disabler_.set_)
				fibers::strategy::interrupt_flags_() |= fibers::detail::INTERRUPTION_BLOCKED;
		}
		catch (...)
		{}
	}
};

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_THIS_FIBER_INTERRUPTION_H
