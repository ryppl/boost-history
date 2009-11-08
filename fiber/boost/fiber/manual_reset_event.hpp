
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBEr_MANUAL_RESET_EVENT_H
#define BOOST_FIBEr_MANUAL_RESET_EVENT_H

#include <boost/cstdint.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/mutex.hpp>

namespace boost {
namespace fiber {

class manual_reset_event : private noncopyable
{
private:
	enum state_t
	{
		RESET = 0,
		SET
	};

	volatile uint32_t	state_;
	volatile uint32_t	waiters_;
	mutex				enter_mtx_;

public:
	explicit manual_reset_event( bool = false);

	void set();

	void reset();

	void wait();

	bool wait( system_time const&);

	template< typename TimeDuration >
	bool wait( TimeDuration const& rel_time)
	{ return wait( get_system_time() + rel_time); }

	bool try_wait();
};

}}

#endif // BOOST_FIBER_MANUAL_RESET_EVENT_H
