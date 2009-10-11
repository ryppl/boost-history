
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_ATOMIC_GCC_H
#define BOOST_TASK_DETAIL_ATOMIC_GCC_H

// based on boost/smart_ptr/detail/atomic_count_gcc.hpp

# if __GNUC__ * 100 + __GNUC_MINOR__ >= 402
#include <ext/atomicity.h> 
# else 
#include <bits/atomicity.h>
# endif

#include <boost/assert.hpp>
#include <boost/cstdint.hpp>
#include <boost/interprocess/detail/atomic.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{
namespace detail
{

#if defined(__GLIBCXX__) // g++ 3.4+

using __gnu_cxx::__atomic_add;
using __gnu_cxx::__exchange_and_add;

#endif

inline
uint32_t atomic_load( uint32_t const volatile * object)
{ return * object; }

inline
void atomic_exchange( uint32_t volatile * object, uint32_t desired)
{
  // inline asm xchg for i386 || x86_64?
  * object = desired;
}

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
uint32_t atomic_fetch_add( uint32_t volatile * object, uint32_t operand)
{
	BOOST_ASSERT( operand == 1);
	return __exchange_and_add( ( _Atomic_word volatile *) object, 1) + 1;
}

inline
uint32_t atomic_fetch_sub( uint32_t volatile * object, uint32_t operand)
{
	BOOST_ASSERT( operand == 1);
	return __exchange_and_add( ( _Atomic_word volatile *) object, -1) - 1;
}
} } }

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_DETAIL_ATOMIC_GCC_H

