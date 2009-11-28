
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_ROUND_ROBIN_H
#define BOOST_FIBERS_ROUND_ROBIN_H

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
#include <boost/fiber/strategy.hpp>

#include <boost/config/abi_prefix.hpp>

# if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable:4251 4275)
# endif

namespace boost {
namespace fibers {

class BOOST_FIBER_DECL round_robin : private noncopyable,
									 public strategy
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

	container			fibers_;
	runnable_queue		runnable_fibers_;
	terminated_queue	terminated_fibers_;

public:
	round_robin();

	void add( fiber);

	void yield( fiber::id const&);

	void cancel( fiber::id const&);

	void join( fiber::id const&);

	void interrupt( fiber::id const&);

	void reschedule( fiber::id const&);

	bool run();

	bool empty();

	std::size_t size();
};

}}

# if defined(BOOST_MSVC)
# pragma warning(pop)
# endif

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_ROUND_ROBIN_H
