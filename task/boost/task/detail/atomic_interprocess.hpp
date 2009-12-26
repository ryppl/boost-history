
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASKS_DETAIL_ATOMIC_INTERPROCESS_H
#define BOOST_TASKS_DETAIL_ATOMIC_INTERPROCESS_H

#include <boost/assert.hpp>
#include <boost/cstdint.hpp>
#include <boost/interprocess/detail/atomic.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace tasks {
namespace detail {

inline
uint32_t atomic_load( uint32_t const volatile * object)
{ return * object; }

inline
void atomic_exchange( uint32_t volatile * object, uint32_t desired)
{ interprocess::detail::atomic_write32( object, desired); }

inline
bool atomic_compare_exchange_strong( uint32_t volatile * object, uint32_t * expected, uint32_t desired)
{
	uint32_t prev = interprocess::detail::atomic_cas32( object, desired, * expected);
	if ( prev != * expected)
	{
		* expected = prev;
		return false;
	}
	return true;
}

inline
unsigned int atomic_fetch_add( uint32_t volatile * object, uint32_t operand)
{
	BOOST_ASSERT( operand == 1);
	return interprocess::detail::atomic_inc32( object);
}

inline
unsigned int atomic_fetch_sub( uint32_t volatile * object, uint32_t operand)
{
	BOOST_ASSERT( operand == 1);
	return interprocess::detail::atomic_dec32( object);
}

}}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASKS_DETAIL_ATOMIC_INTERPROCESS_H

