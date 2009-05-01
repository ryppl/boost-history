
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/default_pool.hpp"

#include <boost/thread.hpp>

#include "boost/task/poolsize.hpp"

namespace boost { namespace task
{
namespace detail
{
default_pool_t
static_pool::instance( poolsize( thread::hardware_concurrency() ) );
}
} }
