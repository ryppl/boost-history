
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_COUNT_DOWN_EVENT_H
#define BOOST_FIBERS_COUNT_DOWN_EVENT_H

#include <boost/cstdint.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/object/id.hpp>
#include <boost/fiber/mutex.hpp>
#include <boost/fiber/scheduler.hpp>
#include <boost/fiber/strategy.hpp>

namespace boost {
namespace fibers {

class count_down_event : private noncopyable
{
private:
	uint32_t			initial_;
	volatile uint32_t	current_;
	object::id			id_;
	strategy::ptr_t		strategy_;

public:
	template< typename Strategy >
	count_down_event( scheduler< Strategy > & sched, uint32_t initial) :
		initial_( initial),
		current_( initial_),
		id_( * this),
		strategy_( sched.strategy_)
	{ strategy_->register_object( id_); }

	~count_down_event();

	uint32_t initial() const;

	uint32_t current() const;

	bool is_set() const;

	void set();

	void wait();
};

}}

#endif // BOOST_FIBERS_COUNT_DOWN_EVENT_H
