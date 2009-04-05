//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_DEFAULT_POOL_H
#define BOOST_TP_DEFAULT_POOL_H

#include <boost/tp/fifo.hpp>
#include <boost/tp/pool.hpp>
#include <boost/tp/task.hpp>
#include <boost/tp/unbounded_channel.hpp>

namespace boost { namespace tp
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

template< typename Act >
task< typename result_of< Act() >::type > lauch_in_pool( Act const& act)
{ return get_default_pool().submit( act); }
} }

#endif // BOOST_TP_DEFAULT_POOL_H
