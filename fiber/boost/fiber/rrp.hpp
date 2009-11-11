
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_RRP_H
#define BOOST_FIBERS_RRP_H

#include <cstddef>
#include <memory>
#include <queue>

#include <boost/ptr_container/ptr_map.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/fiber.hpp>
#include <boost/fiber/policy.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fibers {

class BOOST_FIBER_DECL rrp : private noncopyable,
							 public policy
{
private:
	fiber						master_;
	fiber::id					f_id_;
	ptr_map< fiber::id, fiber >	fibers_;
	std::queue< fiber::id >		runnable_fibers_;
	std::queue< fiber::id >		dead_fibers_;

public:
	rrp();

	void add_fiber( std::auto_ptr< fiber >);

	fiber::id active_fiber();

	void yield_active_fiber();

	void exit_active_fiber();

	bool run();

	bool empty();

	std::size_t size();
};

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_RRP_H
