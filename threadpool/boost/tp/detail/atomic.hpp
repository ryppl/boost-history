//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_DETAIL_ATOMIC_H
#define BOOST_TP_DETAIL_ATOMIC_H

#include <boost/config.hpp>

namespace boost { namespace tp {
namespace detail
{
#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

#include <Windows.h>

template< typename T >
bool atomic_compare_exchange_ptr( volatile T * object, T expected, T desired)
{ return InterlockedCompareExchangePointer( object, expected, desired); }

void atomic_write_32( volatile unsigned int * object, unsigned int desired)
{ InterlockedExchange( object, desired); }

template< typename T >
void atomic_write_ptr( volatile T * object, T desired)
{ InterlockedExchangePointer( object, desired); }

inline
unsigned int atomic_inc_32( volatile unsigned int * object)
{ return InterlockedIncrement( object); }

inline
unsigned int atomic_dec_32( volatile unsigned int * object)
{ return InterlockedDecrement( object); }

#elif defined(__hpux)

#include <atomic.h>

template< typename T >
bool atomic_compare_exchange_ptr( volatile T * object, T expected, T desired)
{ return ::atomic_cas( object, expected, & desired) == object; }

inline
void atomic_write_32( volatile unsigned int * object, unsigned int desired)
{ * object = desired; }

template< typename T >
void atomic_write_ptr( volatile T * object, T * desired)
{ * object = desired; }

inline
unsigned int atomic_inc_32( volatile unsigned int * object)
{ return ::atomic_inc( object); }

inline
unsigned int atomic_dec_32( volatile unsigned int * object)
{ return ::atomic_dec( object); }

#elif (defined(sun) || defined(__sun))

#include <atomic.h>

template< typename T >
bool atomic_compare_exchange_ptr( volatile T * object, T expected, T desired)
{ return ::atomic_cas_ptr( object, expected, & desired) == object; }

inline
void atomic_write_32( volatile unsigned int * object, unsigned int desired)
{
	unsigned int desired_( desired);
	::atomic_swap_32( object, desired_);
}

template< typename T >
void atomic_write_ptr( volatile T * object, T * desired)
{ * object = desired; }

inline
unsigned int atomic_inc_32( volatile unsigned int * object)
{ return ::atomic_inc_32( object); }

inline
unsigned int atomic_dec_32( volatile unsigned int * object)
{ return ::atomic_dec_32( object); }

#elif defined(__GNUC__) && ( __GNUC__ * 100 + __GNUC_MINOR__ >= 401 )

template< typename T >
bool atomic_compare_exchange_ptr( volatile T * object, T expected, T desired)
{ return __sync_bool_compare_and_swap( object, expected, desired); }

inline
void atomic_write_32( volatile unsigned int * object, unsigned int desired)
{ * object = desired; }

template< typename T >
void atomic_write_ptr( volatile T * object, T desired)
{ * object = desired; }

inline
unsigned int atomic_inc_32( volatile unsigned int * object)
{ return __sync_fetch_and_add( object, 1); }

inline
unsigned int atomic_dec_32( volatile unsigned int * object)
{ return __sync_fetch_and_sub( object, 1); }

#else

#error No atomic operations available for this platform!

#endif
} } }

#endif // BOOST_TP_DETAIL_ATOMIC_H

