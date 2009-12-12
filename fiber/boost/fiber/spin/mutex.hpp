
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
//  based on boost::interprocess::sync::interprocess_spin_mutex

#ifndef BOOST_FIBERS_SPIN_MUTEX_H
#define BOOST_FIBERS_SPIN_MUTEX_H

#include <boost/cstdint.hpp>
#include <boost/thread/locks.hpp>
#include <boost/utility.hpp>

namespace boost {
namespace fibers {
namespace spin {

class mutex : private noncopyable
{
private:
	volatile uint32_t	state_;

public:
	typedef unique_lock< mutex >	scoped_lock;

	mutex();

	void lock();

	bool try_lock();

	void unlock();
};

typedef mutex try_mutex;

}}}

#endif // BOOST_FIBERS_SPIN_MUTEX_H
