
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

#include <boost/utility.hpp>

#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/fiber.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fibers {
namespace detail {

class BOOST_FIBER_DECL scheduler_impl : private noncopyable
{
private:
	typedef std::map< fiber::id, fiber >	container;
	typedef std::list< fiber::id >			runnable_queue;
	typedef std::queue< fiber::id >		terminated_queue;

	fiber		master_;
	fiber::id	f_id_;
	container	fibers_;
	runnable_queue		runnable_fibers_;
	terminated_queue	terminated_fibers_;

public:
	scheduler_impl();

	void add_fiber( fiber);

	fiber::id active_fiber();

	void yield_active_fiber();

	void cancel_active_fiber();

	void suspend_active_fiber();

	void cancel_fiber( fiber::id const&);

	void suspend_fiber( fiber::id const&);

	void resume_fiber( fiber::id const&);

	bool run();

	bool empty();

	std::size_t size();
};

}}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_DETAIL_SCHEDULER_IMPL_H
