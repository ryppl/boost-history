
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_ATOMIC_INTERPROCESS_H
#define BOOST_TASK_DETAIL_ATOMIC_INTERPROCESS_H

#include <boost/assert.hpp>
#include <boost/cstdint.hpp>
#include <boost/interprocess/detail/atomic.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{
namespace detail
{
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

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_DETAIL_ATOMIC_INTERPROCESS_H

