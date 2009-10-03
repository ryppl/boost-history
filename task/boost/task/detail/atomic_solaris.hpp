
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_ATOMIC_SOLARIS_H
#define BOOST_TASK_DETAIL_ATOMIC_SOLARIS_H

extern "C"
{
#include <atomic.h>
}

#include <boost/assert.hpp>
#include <boost/cstdint.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{
namespace detail
{
inline
void atomic_exchange( uint32_t volatile * object, uint32_t desired)
{ * object = desired; }

inline
bool atomic_compare_exchange_strong( uint32_t volatile * object, uint32_t * expected, uint32_t desired)
{
	uint32_t prev = ::atomic_cas_32( object, * expected, desired);
	if ( prev != * expected)
	{
		* expected = prev;
		return false;
	}
	return true;
}

inline
uint32_t atomic_fetch_add( uint32_t volatile * object, uint32_t operand)
{
	BOOST_ASSERT( operand == 1);
	return ::atomic_inc_32( object);
}

inline
uint32_t atomic_fetch_sub( uint32_t volatile * object, uint32_t operand)
{
	BOOST_ASSERT( operand == 1);
	return ::atomic_dec_32( object);
}
} } }

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_DETAIL_ATOMIC_SOLARIS_H

