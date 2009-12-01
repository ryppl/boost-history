
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/fiber/auto_reset_event.hpp"

#include <boost/fiber/detail/atomic.hpp>
#include <boost/fiber/utility.hpp>

namespace boost {
namespace fibers {

auto_reset_event::auto_reset_event( bool isset) :
	state_(
		isset ?
		static_cast< uint32_t >( SET) :
		static_cast< uint32_t >( RESET) )
{}

void
auto_reset_event::set()
{ detail::atomic_exchange( & state_, static_cast< uint32_t >( SET) ); }

void
auto_reset_event::wait()
{
	uint32_t expected = static_cast< uint32_t >( SET);
	while ( ! detail::atomic_compare_exchange_strong(
			& state_, & expected,
			static_cast< uint32_t >( RESET) ) )
	{
		this_fiber::interruption_point();
		this_fiber::yield();
		this_fiber::interruption_point();
		expected = static_cast< uint32_t >( SET);
	}
}

bool
auto_reset_event::try_wait()
{
	uint32_t expected = static_cast< uint32_t >( SET);
	return detail::atomic_compare_exchange_strong(
			& state_, & expected,
			static_cast< uint32_t >( RESET) );
}

}}
