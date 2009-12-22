
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_SPIN_MANUAL_RESET_EVENT_H
#define BOOST_FIBERS_SPIN_MANUAL_RESET_EVENT_H

#include <cstddef>

#include <boost/atomic.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/spin/mutex.hpp>

namespace boost {
namespace fibers {
namespace spin {

class manual_reset_event : private noncopyable
{
private:
	enum state
	{
		SET = 0,
		RESET
	};

	atomic< state >			state_;
	atomic< std::size_t >	waiters_;
	mutex					enter_mtx_;

public:
	explicit manual_reset_event( bool = false);

	void set();

	void reset();

	void wait();

	bool try_wait();
};

}}}

#endif // BOOST_FIBERS_SPIN_MANUAL_RESET_EVENT_H
