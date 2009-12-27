
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASKS_ASYNC_H
#define BOOST_TASKS_ASYNC_H

#include <boost/config.hpp>
#include <boost/move/move.hpp>

#include <boost/task/as_sub_task.hpp>
#include <boost/task/handle.hpp>
#include <boost/task/static_pool.hpp>
#include <boost/task/task.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace tasks {

template< typename R >
handle< R > async( task< R > t)
{ return as_sub_task()( boost::move( t) ); }

template< typename R >
handle< R > async( BOOST_RV_REF( task< R >) t)
{ return as_sub_task()( t); }

template< typename R, typename EP >
handle< R > async( task< R > t, EP ep)
{ return ep( boost::move( t) ); }

template< typename R, typename EP >
handle< R > async( BOOST_RV_REF( task< R >) t, EP ep)
{ return ep( t); }

template< typename R, typename Queue, typename UMS >
handle< R > async( task< R > t, static_pool< Queue, UMS > & pool)
{ return pool.submit( boost::move( t) ); }

template< typename R, typename Queue, typename UMS >
handle< R > async( BOOST_RV_REF( task< R >) t, static_pool< Queue, UMS > & pool)
{ return pool.submit( t); }

template< typename R, typename Attr, typename Queue, typename UMS >
handle< R > async( task< R > t, Attr attr, static_pool< Queue, UMS > & pool)
{ return pool.submit( boost::move( t), attr); }

template< typename R, typename Attr, typename Queue, typename UMS >
handle< R > async( BOOST_RV_REF( task< R >) t, Attr attr, static_pool< Queue, UMS > & pool)
{ return pool.submit( t, attr); }

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASKS_ASYNC_H
