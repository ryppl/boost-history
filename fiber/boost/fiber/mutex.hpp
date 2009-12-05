
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
//  based on boost::interprocess::sync::interprocess_mutex

#ifndef BOOST_FIBERS_MUTEX_H
#define BOOST_FIBERS_MUTEX_H

#include <boost/cstdint.hpp>
#include <boost/thread/locks.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/object/id.hpp>

namespace boost {
namespace fibers {

class mutex : private noncopyable
{
private:
	volatile uint32_t	state_;
	object::id			id_;

public:
	typedef unique_lock< mutex >			scoped_lock;

	mutex();

	~mutex();

	void lock();

	bool try_lock();

	void unlock();
};

typedef mutex try_mutex;

}}

#endif // BOOST_FIBERS_MUTEX_H
