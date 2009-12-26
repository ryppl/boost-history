
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/detail/guard.hpp"

#include <boost/assert.hpp>

#include <boost/task/detail/atomic.hpp>

namespace boost {
namespace tasks {
namespace detail {

guard::guard( volatile uint32_t & active_worker) :
	active_worker_( active_worker)
{
	BOOST_ASSERT( active_worker_ >= 0);
	atomic_fetch_add( & active_worker_, 1);
}

guard::~guard()
{
	atomic_fetch_sub( & active_worker_, 1);
	BOOST_ASSERT( active_worker_ >= 0);
}

}}}
