
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_AUTO_RESET_EVENT_H
#define BOOST_FIBERS_AUTO_RESET_EVENT_H

#include <boost/cstdint.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/object/id.hpp>

namespace boost {
namespace fibers {

class auto_reset_event : private noncopyable
{
private:
	enum state_t
	{
		RESET = 0,
		SET
	};

	volatile uint32_t	state_;
	object::id			id_;

public:
	explicit auto_reset_event( bool = false);

	~auto_reset_event();

	void set();

	void wait();

	bool try_wait();
};

}}

#endif // BOOST_FIBERS_AUTO_RESET_EVENT_H
