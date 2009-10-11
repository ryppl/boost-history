
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_ATOMIC_GCC_X86_H
#define BOOST_TASK_DETAIL_ATOMIC_GCC_X86_H

#include <boost/assert.hpp>
#include <boost/cstdint.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace task {
namespace detail {

inline
uint32_t atomic_load( uint32_t const volatile * object)
{ return * object; }

inline
void atomic_exchange( uint32_t volatile * object, uint32_t desired)
{
	__asm__ __volatile__
	(
		"xchg %0, %1" :
		"+r" ( desired), "+m" ( * object)
	);
}

inline
bool atomic_compare_exchange_strong( uint32_t volatile * object, uint32_t * expected, uint32_t desired)
{
	uint32_t prev = * expected;

	__asm__ __volatile__
	(
		"lock\n\t"
		"cmpxchg %3, %1"
		: "=a" ( * expected), "=m" ( * object)
		: "a" ( prev), "r" ( desired)
		: "memory", "cc"
	);

	return prev == * expected;
}

inline
long atomic_fetch_add( uint32_t volatile * object, uint32_t operand)
{
	int operand_ = static_cast< int >( operand);
	int r;

	__asm__ __volatile__
	(
		"lock\n\t"
		"xadd %1, %0" :
		"+m"( * object), "=r"( r):
		"1"( operand_):
		"memory", "cc"
	);

	return r;
}

inline
long atomic_fetch_sub( uint32_t volatile * object, uint32_t operand)
{
	int operand_ = static_cast< int >( -1 * operand);
	int r;

	__asm__ __volatile__
	(
		"lock\n\t"
		"xadd %1, %0":
		"+m"( * object), "=r"( r ):
		"1"( operand_):
		"memory", "cc"
	);

	return r;
}

}}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_DETAIL_ATOMIC_GCC_X86_H

