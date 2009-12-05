
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fiber/mutex.hpp>

#include <boost/fiber/detail/atomic.hpp>
#include <boost/fiber/strategy.hpp>

namespace boost {
namespace fibers {

mutex::mutex() :
	state_( 0),
	id_( * this)
{ strategy::register_object_( id_); }

mutex::~mutex()
{ strategy::unregister_object_( id_); }

void
mutex::lock()
{
	for (;;)
	{
		uint32_t expected = 0;
		if ( detail::atomic_compare_exchange_strong( & state_, & expected, 1) )
			break;
		else
			strategy::wait_for_object_( id_);
	}
}

bool
mutex::try_lock()
{
	uint32_t expected = 0;
	return detail::atomic_compare_exchange_strong( & state_, & expected, 1);
}

void
mutex::unlock()
{
	detail::atomic_exchange( & state_, 0);
	strategy::object_notify_one_( id_);
}

}}
