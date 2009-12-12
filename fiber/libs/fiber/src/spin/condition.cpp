
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/fiber/spin/condition.hpp"

namespace boost {
namespace fibers {
namespace spin {

void
condition::notify_( uint32_t cmd)
{
	enter_mtx_.lock();

	if ( 0 == detail::atomic_load( & waiters_) )
	{
		enter_mtx_.unlock();
		return;
	}

	uint32_t expected = static_cast< uint32_t >( SLEEPING);
	while ( ! detail::atomic_compare_exchange_strong(
				& cmd_, & expected, cmd) )
		this_fiber::yield();	
}

condition::condition() :
	cmd_( static_cast< uint32_t >( SLEEPING) ),
	waiters_( 0),
	enter_mtx_(),
	check_mtx_()
{}

void
condition::notify_one()
{ notify_( static_cast< uint32_t >( NOTIFY_ONE) ); }

void
condition::notify_all()
{ notify_( static_cast< uint32_t >( NOTIFY_ALL) ); }

}}}
