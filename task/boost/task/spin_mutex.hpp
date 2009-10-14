
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
//  based on boost::interprocess::sync::interprocess_mutex

#ifndef BOOST_TASK_SPIN_MUTEX_H
#define BOOST_TASK_SPIN_MUTEX_H

#include <boost/cstdint.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/utility.hpp>

#include <boost/task/spin_unique_lock.hpp>

namespace boost {
namespace task {

class spin_mutex : private noncopyable
{
private:
	volatile uint32_t	state_;

public:
	typedef spin_unique_lock< spin_mutex >		scoped_lock;

	spin_mutex();

	void lock();

	bool try_lock();

	bool timed_lock( system_time const& abs_time);

	template< typename TimeDuration >
	bool timed_lock( TimeDuration const& rel_time)
	{ return timed_lock( get_system_time() + rel_time); }

	void unlock();
};

}}

#endif // BOOST_TASK_SPIN_MUTEX_H
