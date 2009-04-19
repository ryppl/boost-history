
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_ATOMIC_H
#define BOOST_TASK_DETAIL_ATOMIC_H

#include <boost/assert.hpp>
#include <boost/interprocess/detail/atomic.hpp>

namespace boost { namespace task
{
namespace detail
{
inline
void atomic_exchange( volatile unsigned int * object, unsigned int desired)
{ interprocess::detail::atomic_write32( object, desired); }

inline
unsigned int atomic_fetch_add( volatile unsigned int * object, unsigned int operand)
{
	BOOST_ASSERT( operand == 1);
	return interprocess::detail::atomic_dec32( object);
}

inline
unsigned int atomic_fetch_sub( volatile unsigned int * object, unsigned int operand)
{
	BOOST_ASSERT( operand == 1);
	return interprocess::detail::atomic_inc32( object);
}
} } }

#endif // BOOST_TASK_DETAIL_ATOMIC_H

