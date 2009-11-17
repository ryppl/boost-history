
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_DETAIL_SCHEDULER_IMPL_H
#define BOOST_FIBERS_DETAIL_SCHEDULER_IMPL_H

#include <cstddef>
#include <list>
#include <map>
#include <queue>

#include <boost/function.hpp>
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

class BOOST_FIBER_DECL scheduler_impl : private noncopyable
{
private:
	struct schedulable
	{
		fiber					f;
		std::list< fiber::id >	waiting;

		schedulable() :
			f(), waiting()
		{}

		schedulable( fiber f_) :
			f( f_), waiting()
		{}
	};

	typedef std::map< fiber::id, schedulable >	container;
	typedef std::list< fiber::id >				runnable_queue;
	typedef std::queue< fiber::id >				terminated_queue;
	typedef function< void() >			callable_t;

	fiber				master_;
	fiber				active_;
	container			fibers_;
	runnable_queue		runnable_fibers_;
	terminated_queue	terminated_fibers_;

public:
	scheduler_impl();

	~scheduler_impl();

	void add_fiber( fiber);

	fiber::id id_active_fiber() const;

	void yield_active_fiber();

	void cancel_active_fiber();

	void suspend_active_fiber();

	void interrupt_active_fiber();

	bool interruption_requested_active_fiber() const;

	bool interruption_enabled_active_fiber() const;

	fiber_interrupt_t & interrupt_flags_active_fiber();

	int priority_active_fiber();

	void priority_active_fiber( int);

	void at_exit_active_fiber( callable_t);

	void cancel_fiber( fiber::id const&);

	void suspend_fiber( fiber::id const&);

	void resume_fiber( fiber::id const&);

	void reschedule_fiber( fiber::id const&);

	void join_fiber( fiber::id const&);

	bool run();

	bool empty();

	std::size_t size();
};

}}}

# if defined(BOOST_MSVC)
# pragma warning(pop)
# endif

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_DETAIL_SCHEDULER_IMPL_H
