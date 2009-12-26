
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASKS_DETAIL_ATOMIC_SYNC_H
#define BOOST_TASKS_DETAIL_ATOMIC_SYNC_H

// based on boost/smart_ptr/detail/atomic_count_gc.hpp

# if defined( __ia64__ ) && defined( __INTEL_COMPILER )
extern "C"
{
#include<ia64intrin.h>
}
# endif

#include <boost/assert.hpp>
#include <boost/cstdint.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace tasks {
namespace detail {

inline
uint32_t atomic_load( uint32_t const volatile * object)
{ return * object; }

inline
void atomic_exchange( uint32_t volatile * object, uint32_t desired)
{ * object = desired; }

inline
bool atomic_compare_exchange_strong( uint32_t volatile * object, uint32_t * expected, uint32_t desired)
{
	uint32_t prev = __sync_val_compare_and_swap( object, * expected, desired);
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
	return __sync_fetch_and_add( object, 1);
}

inline
uint32_t atomic_fetch_sub( uint32_t volatile * object, uint32_t operand)
{
	BOOST_ASSERT( operand == 1);
	return __sync_fetch_and_add( object, -1);
}

}}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASKS_DETAIL_ATOMIC_SYNC_H

