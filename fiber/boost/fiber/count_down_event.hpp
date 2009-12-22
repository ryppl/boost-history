
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_COUNT_DOWN_EVENT_H
#define BOOST_FIBERS_COUNT_DOWN_EVENT_H

#include <cstddef>

#include <boost/utility.hpp>

#include <boost/fiber/object/id.hpp>
#include <boost/fiber/mutex.hpp>
#include <boost/fiber/scheduler.hpp>
#include <boost/fiber/strategy.hpp>

# if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable:4355)
# endif

namespace boost {
namespace fibers {

class count_down_event : private noncopyable
{
private:
	std::size_t		initial_;
	std::size_t		current_;
	object::id		id_;
	strategy::ptr	strategy_;

public:
	template< typename Strategy >
	count_down_event( scheduler< Strategy > & sched, std::size_t initial) :
		initial_( initial),
		current_( initial_),
		id_( * this),
		strategy_( sched.strategy_)
	{ strategy_->register_object( id_); }

	~count_down_event();

	std::size_t initial() const;

	std::size_t current() const;

	bool is_set() const;

	void set();

	void wait();
};

}}

# if defined(BOOST_MSVC)
# pragma warning(pop)
# endif

#endif // BOOST_FIBERS_COUNT_DOWN_EVENT_H
