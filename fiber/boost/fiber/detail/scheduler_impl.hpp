
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_DETAIL_SCHEDULER_IMPL_H
#define BOOST_FIBERS_DETAIL_SCHEDULER_IMPL_H

#include <cstddef>
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
	fiber							master_;
	fiber::id						f_id_;
	std::map< fiber::id, fiber >	fibers_;
	std::queue< fiber::id >			runnable_fibers_;
	std::queue< fiber::id >			terminated_fibers_;

public:
	scheduler_impl();

	void add_fiber( fiber);

	fiber::id active_fiber();

	void yield_active_fiber();

	void terminate_active_fiber();

	bool run();

	bool empty();

	std::size_t size();
};

}}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_DETAIL_SCHEDULER_IMPL_H
