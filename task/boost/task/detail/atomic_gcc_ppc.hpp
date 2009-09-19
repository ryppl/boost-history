
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_ATOMIC_GCC_PPC_H
#define BOOST_TASK_DETAIL_ATOMIC_GCC_PPC_H

#include <boost/assert.hpp>
#include <boost/cstdint.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{
namespace detail
{
inline
void atomic_exchange( uint32_t volatile * object, uint32_t desired)
{
	uint32_t r;

	__asm__ __volatile__
	(
		"0:\n\t"
		"lwarx   %0, 0, %2 \n\t"
		"stwcx.  %1, 0, %2 \n\t"
		"bne-     1b" :
		"=r" ( r) :
		"r" ( desired), "r" ( object)
	);
}

inline
uint32_t atomic_fetch_add( uint32_t volatile * object, uint32_t operand)
{
	int object_ = static_cast< int >( object);
	int operand_ = static_cast< int >( operand);
	int r, t;

	__asm__ __volatile__
	(
		"0:\n\t"
		"lwarx  %0,0,%2\n\t"
		"add    %1,%0,%3\n\t"
		"stwcx. %1,0,%2\n\t"
		"bne-   0b"
		: "=&r" ( r), "=&r" ( t)
		: "b" ( object_), "r" ( operand_)
		: "memory", "cc"
	);

	return r;
}

inline
uint32_t atomic_fetch_sub( uint32_t volatile * object, uint32_t operand)
{
	int object_ = static_cast< int >( object);
	int operand_ = static_cast< int >( -1 * operand);
	int r;

	__asm__ __volatile__
	(
		"0:\n\t"
		"lwarx  %0,0,%2\n\t"
		"add    %1,%0,%3\n\t"
		"stwcx. %1,0,%2\n\t"
		"bne-   0b"
		: "=&r" ( r), "=&r" ( t)
		: "b" ( object_), "r" ( operand_)
		: "memory", "cc"
	);

	return r;
}
} } }

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_DETAIL_ATOMIC_GCC_PPC_H

