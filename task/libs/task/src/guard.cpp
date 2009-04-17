
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/detail/guard.hpp"

#include <boost/assert.hpp>

#include <boost/task/detail/atomic.hpp>

namespace boost { namespace task
{
namespace detail
{
guard::guard( unsigned int & active_worker)
: active_worker_( active_worker)
{
	BOOST_ASSERT( active_worker_ >= 0);
	atomic_inc_32( & active_worker_);
}

guard::~guard()
{
	atomic_dec_32( & active_worker_);
	BOOST_ASSERT( active_worker_ >= 0);
}
} } }

