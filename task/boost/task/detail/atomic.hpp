
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_ATOMIC_H
#define BOOST_TASK_DETAIL_ATOMIC_H

#include <boost/assert.hpp>
#include <boost/cstdint.hpp>
#include <boost/interprocess/detail/atomic.hpp>

namespace boost { namespace task
{
namespace detail
{
//TODO: use interlocked_exchange on Windows
// and inline assembler on otherplatforms (XCHG etc.)
inline
void atomic_exchange( volatile uint32_t * object, uint32_t desired)
{ interprocess::detail::atomic_write32( object, desired); }

inline
unsigned int atomic_fetch_add( volatile uint32_t * object, uint32_t operand)
{
	BOOST_ASSERT( operand == 1);
	return interprocess::detail::atomic_inc32( object);
}

inline
unsigned int atomic_fetch_sub( volatile uint32_t * object, uint32_t operand)
{
	BOOST_ASSERT( operand == 1);
	return interprocess::detail::atomic_dec32( object);
}
} } }

#endif // BOOST_TASK_DETAIL_ATOMIC_H

