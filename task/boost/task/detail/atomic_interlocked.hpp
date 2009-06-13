
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_ATOMIC_INTERLOCKED_H
#define BOOST_TASK_DETAIL_ATOMIC_INTERLOCKED_H

#include <boost/assert.hpp>
#include <boost/cstdint.hpp>
#include <boost/detail/interlocked.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{
namespace detail
{
inline
void atomic_exchange( uint32_t volatile * object, uint32_t desired)
{ BOOST_INTERLOCKED_EXCHANGE( reinterpret_cast< long volatile * >( object), desired); }

inline
uint32_t atomic_fetch_add( uint32_t volatile * object, uint32_t operand)
{
	BOOST_ASSERT( operand == 1);
	return BOOST_INTERLOCKED_INCREMENT( reinterpret_cast< long volatile * >( object) );
}

inline
uint32_t atomic_fetch_sub( uint32_t volatile * object, uint32_t operand)
{
	BOOST_ASSERT( operand == 1);
	return BOOST_INTERLOCKED_DECREMENT( reinterpret_cast< long volatile * >( object) );
}
} } }

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_DETAIL_ATOMIC_INTERLOCKED_H

