
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/fiber/condition.hpp"

#include <boost/fiber/utility.hpp>

namespace boost {
namespace fibers {

condition::~condition()
{ strategy_->unregister_object( id_); }

void
condition::notify_one()
{
	enter_mtx_.lock();

	if ( 0 == detail::atomic_load( & waiters_) )
	{
		enter_mtx_.unlock();
		return;
	}

	uint32_t cmd = static_cast< uint32_t >( NOTIFY_ONE);
	uint32_t expected = static_cast< uint32_t >( SLEEPING);
	while ( ! detail::atomic_compare_exchange_strong(
				& cmd_, & expected, cmd) )
		this_fiber::yield();
	
	strategy_->object_notify_one( id_);
}

void
condition::notify_all()
{
	enter_mtx_.lock();

	if ( 0 == detail::atomic_load( & waiters_) )
	{
		enter_mtx_.unlock();
		return;
	}

	uint32_t cmd = static_cast< uint32_t >( NOTIFY_ALL);
	uint32_t expected = static_cast< uint32_t >( SLEEPING);
	while ( ! detail::atomic_compare_exchange_strong(
				& cmd_, & expected, cmd) )
		this_fiber::yield();

	strategy_->object_notify_all( id_);
}

}}
