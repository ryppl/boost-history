
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DEFAULT_POOL_H
#define BOOST_TASK_DEFAULT_POOL_H

#include <boost/task/fifo.hpp>
#include <boost/task/pool.hpp>
#include <boost/task/unbounded_channel.hpp>

namespace boost { namespace task
{
typedef pool< unbounded_channel< fifo > > default_pool;

namespace detail
{
struct static_pool
{ static default_pool instance; };
}

inline
default_pool & get_default_pool()
{ return detail::static_pool::instance; }
} }

#endif // BOOST_TASK_DEFAULT_POOL_H
