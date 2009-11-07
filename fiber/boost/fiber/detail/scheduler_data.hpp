
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBER_DETAIL_SCHEDULER_DATA_H
#define BOOST_FIBER_DETAIL_SCHEDULER_DATA_H

#include <memory>
#include <queue>
#include <utility>

#include <boost/ptr_container/ptr_map.hpp>

#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/fiber.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fiber {
namespace detail {

struct BOOST_FIBER_DECL scheduler_data
{
	fiber						master;
	fiber::id					f_id;
	ptr_map< fiber::id, fiber >	fibers;
	std::queue< fiber::id >		runnable_fibers;
	std::queue< fiber::id >		dead_fibers;

	scheduler_data() :
		master(),
		f_id(),
		fibers(),
		runnable_fibers(),
		dead_fibers()
	{}
};

}}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBER_DETAIL_SCHEDULER_DATA_H
