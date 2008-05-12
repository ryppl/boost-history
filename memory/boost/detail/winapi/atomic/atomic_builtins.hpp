//
//  boost/detail/winapi/atomic/atomic_builtins.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/detail/todo.htm for documentation.
//
#ifndef BOOST_DETAIL_WINAPI_ATOMIC_ATOMIC_BUILTINS_HPP
#define BOOST_DETAIL_WINAPI_ATOMIC_ATOMIC_BUILTINS_HPP

// -------------------------------------------------------------------------

__forceinline LONG WINAPI InterlockedIncrement(volatile LPLONG lpAddend)
{
	return __sync_add_and_fetch(lpAddend, 1);
}

__forceinline LONG WINAPI InterlockedDecrement(volatile LPLONG lpAddend)
{
    return __sync_sub_and_fetch(lpAddend, 1);
}

// -------------------------------------------------------------------------

__forceinline LONG WINAPI InterlockedExchange(volatile LPLONG Target, LONG Value)
{
	__sync_synchronize();
	return __sync_lock_test_and_set(Target, Value);
}

__forceinline PVOID WINAPI InterlockedExchangePointer(volatile PVOID* Target, PVOID Value)
{
	__sync_synchronize();
	return (PVOID)__sync_lock_test_and_set(Target, Value);
}

// -------------------------------------------------------------------------

__forceinline LONG WINAPI InterlockedExchangeAdd(volatile LPLONG Addend, LONG Value)
{
    return __sync_fetch_and_add(Addend, Value);
}

// -------------------------------------------------------------------------

__forceinline LONG WINAPI InterlockedCompareExchange(
    volatile LPLONG Destination, LONG Exchange, LONG Comperand)
{
	return __sync_val_compare_and_swap(Destination, Comperand, Exchange);
}

__forceinline PVOID WINAPI InterlockedCompareExchangePointer(
    volatile PVOID* Destination, PVOID Exchange, PVOID Comperand)
{
	return (PVOID)__sync_val_compare_and_swap(Destination, Comperand, Exchange);
}

// -------------------------------------------------------------------------

__forceinline LONG64 WINAPI InterlockedExchange64(
	volatile PLONG64 Target, LONG64 Value)
{
	__sync_synchronize();
	return __sync_lock_test_and_set(Target, Value);
}

__forceinline bool WINAPI CompareAndSwap64(
	PLONG64 Destination, LONG64 Exchange, LONG64 Comperand)
{
	return __sync_bool_compare_and_swap_8(Destination, Comperand, Exchange);
}

// -------------------------------------------------------------------------

typedef int ATOMIC_LONG128_ __attribute__ ((mode (TI)));
typedef ATOMIC_LONG128_ ATOMIC_LONG128;
typedef ATOMIC_LONG128_* PATOMIC_LONG128;

__forceinline bool WINAPI CompareAndSwap128(
	PATOMIC_LONG128 Destination, ATOMIC_LONG128 Exchange, ATOMIC_LONG128 Comperand)
{
	return __sync_bool_compare_and_swap_16(Destination, Comperand, Exchange);
}

// -------------------------------------------------------------------------
// $Log: $

#endif /* BOOST_DETAIL_WINAPI_ATOMIC_ATOMIC_BUILTINS_HPP */
