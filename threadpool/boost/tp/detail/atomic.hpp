//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_DETAIL_ATOMIC_H
#define BOOST_TP_DETAIL_ATOMIC_H

#include <boost/assert.hpp>
#include <boost/interprocess/detail/atomic.hpp>
#include <boost/utility.hpp>

namespace boost { namespace tp {
namespace detail
{
template< typename A, typename C >
bool atomic_compare_exchange( volatile A * object, C * expected, C desired)
{ return __sync_bool_compare_and_swap( object, * expected, desired); }

template< typename A, typename C >
void atomic_store( volatile A * object, C desired)
{ __sync_lock_test_and_set ( object, desired); }

template< typename A, typename M >
A atomic_fetch_add( volatile A * object, M operand)
{ return __sync_fetch_and_add( object, operand); }

template< typename A, typename M >
A atomic_fetch_sub( volatile A * object, M operand)
{ return __sync_fetch_and_sub( object, operand); }
} } }

#endif // BOOST_TP_DETAIL_ATOMIC_H

