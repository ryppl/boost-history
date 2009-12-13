
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_AUTO_RESET_EVENT_H
#define BOOST_FIBERS_AUTO_RESET_EVENT_H

#include <boost/cstdint.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/object/id.hpp>
#include <boost/fiber/scheduler.hpp>
#include <boost/fiber/strategy.hpp>

# if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable:4355)
# endif

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
	strategy::ptr_t		strategy_;

public:
	template< typename Strategy >
	auto_reset_event( scheduler< Strategy > & sched, bool isset = false) :
		state_(
			isset ?
			static_cast< uint32_t >( SET) :
			static_cast< uint32_t >( RESET) ),
		id_( * this),
		strategy_( sched.strategy_)
	{ strategy_->register_object( id_); }

	~auto_reset_event();

	void set();

	void wait();

	bool try_wait();
};

}}

# if defined(BOOST_MSVC)
# pragma warning(pop)
# endif

#endif // BOOST_FIBERS_AUTO_RESET_EVENT_H
