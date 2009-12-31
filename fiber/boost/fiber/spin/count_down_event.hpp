
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_SPIN_COUNT_DOWN_EVENT_H
#define BOOST_FIBERS_SPIN_COUNT_DOWN_EVENT_H

#include <cstddef>

#include <boost/atomic.hpp>
#include <boost/config.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/detail/config.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace fibers {
namespace spin {

class BOOST_FIBER_DECL count_down_event : private noncopyable
{
private:
	std::size_t				initial_;
	atomic< std::size_t >	current_;

public:
	explicit count_down_event( std::size_t);

	std::size_t initial() const;

	std::size_t current() const;

	bool is_set() const;

	void set();

	void wait();
};

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_FIBERS_SPIN_COUNT_DOWN_EVENT_H
