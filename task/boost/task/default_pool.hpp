
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DEFAULT_POOL_H
#define BOOST_TASK_DEFAULT_POOL_H

#include <boost/task/detail/config.hpp>
#include <boost/task/fifo.hpp>
#include <boost/task/static_pool.hpp>
#include <boost/task/unbounded_channel.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{
typedef static_pool< unbounded_channel< fifo > > default_pool_t;

namespace detail
{
struct static_pool
{ static default_pool_t instance; };
}

inline
default_pool_t & default_pool()
{ return detail::static_pool::instance; }
} }

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_DEFAULT_POOL_H
