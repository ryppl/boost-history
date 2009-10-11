
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_SPIN_AUTO_RESET_EVENT_H
#define BOOST_TASK_SPIN_AUTO_RESET_EVENT_H

#include <boost/cstdint.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/utility.hpp>

namespace boost {
namespace task {

class spin_auto_reset_event : private noncopyable
{
private:
	enum state_t
	{
		RESET = 0,
		SET
	};

	volatile state_t	state_;

public:
	spin_auto_reset_event();

	void set();

	void wait();

	bool wait( system_time const&);

	template< typename TimeDuration >
	bool wait( TimeDuration const& rel_time)
	{ return wait( get_system_time() + rel_time); }
};

}}

#endif // BOOST_TASK_SPIN_AUTO_RESET_EVENT_H
