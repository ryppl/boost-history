
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_DETAIL_STRATEGY_H
#define BOOST_FIBERS_DETAIL_STRATEGY_H

#include <cstddef>
#include <list>
#include <map>
#include <queue>

#include <boost/function.hpp>
#include <boost/optional.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/fiber_state.hpp>
#include <boost/fiber/fiber.hpp>

#include <boost/config/abi_prefix.hpp>

# if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable:4251 4275)
# endif

namespace boost {
namespace fibers {
namespace detail {

class BOOST_FIBER_DECL strategy : private noncopyable
{
private:
	struct schedulable
	{
		fiber					f;
		std::list< fiber::id >	joining_fibers;
		optional< fiber::id >	waiting_on;

		schedulable() :
			f(), joining_fibers(), waiting_on()
		{}

		schedulable( fiber f_) :
			f( f_), joining_fibers(), waiting_on()
		{}
	};

	typedef std::map< fiber::id, schedulable >	container;
	typedef std::list< fiber::id >				runnable_queue;
	typedef std::queue< fiber::id >				terminated_queue;
	typedef function< void() >					callable_t;

	fiber				master_;
	fiber				active_;
	container			fibers_;
	runnable_queue		runnable_fibers_;
	terminated_queue	terminated_fibers_;

public:
	strategy();

	~strategy();

	void add( fiber);

	fiber::id get_id() const;

	void yield();

	void cancel();

	void suspend();

	void interrupt();

	bool interruption_requested();

	bool interruption_enabled();

	fiber_interrupt_t & interrupt_flags();

	int priority();

	void priority( int);

	void at_exit( callable_t);

	void interrupt( fiber::id const&);

	void cancel( fiber::id const&);

	void suspend( fiber::id const&);

	void resume( fiber::id const&);

	void join( fiber::id const&);

	void reschedule( fiber::id const&);

	bool run();

	bool empty();

	std::size_t size();
};

}}}

# if defined(BOOST_MSVC)
# pragma warning(pop)
# endif

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_DETAIL_STRATEGY_H
