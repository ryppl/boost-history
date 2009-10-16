
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_ASYNC_H
#define BOOST_TASK_ASYNC_H

#include <boost/config.hpp>
#include <boost/thread/detail/move.hpp>

#include <boost/task/as_sub_task.hpp>
#include <boost/task/handle.hpp>
#include <boost/task/static_pool.hpp>
#include <boost/task/task.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace task {

template< typename R >
handle< R > async( task< R > t)
{ return as_sub_task()( boost::move( t) ); }

template< typename R, typename EP >
handle< R > async( task< R > t, EP ep)
{ return ep( boost::move( t) ); }

template< typename R, typename Queue >
handle< R > async( task< R > t, static_pool< Queue > & pool)
{ return pool.submit( boost::move( t) ); }

template< typename R, typename Queue, typename Attr >
handle< R > async( task< R > t, Attr attr, static_pool< Queue > & pool)
{ return pool.submit( boost::move( t), attr); }

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_ASYNC_H
