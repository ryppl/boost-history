//  Boost integer/round_pow2.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 


#ifndef BOOST_POWER_2_ROUND
#define BOOST_POWER_2_ROUND

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_integral.hpp>

/*
 *	The functions declared in this file rounds up or down an positive integral to the next power of 2.
 *
 *	The functions `ceil_pow2()' rounds up and the functions
 *		`floor_pow2()' rounds down.
 *
 *	Usage:
 *	
 *	T x = ceil_pow2(y);  // rounds down
 *  T z = floor_pow2(w); // rounds up
 *
 *	Note: for negative integrals, the result is undefined
 */

namespace boost {
	
template <typename T>
typename enable_if_c<is_integral<T>::value && sizeof(T) == 1, T>::type
ceil_pow2(T value)
{
	value = value - 1;
	value = value | (value >> 1);
	value = value | (value >> 2);
	value = value | (value >> 4);

	return value + 1;
}

template <typename T>
typename enable_if_c<is_integral<T>::value && sizeof(T) == 2, T>::type 
ceil_pow2(T value)
{
	value = value - 1;
	value = value | (value >> 1);
	value = value | (value >> 2);
	value = value | (value >> 4);
	value = value | (value >> 8);
	
	return value + 1;
}
	
template <typename T>
typename enable_if_c<is_integral<T>::value && sizeof(T) == 4, T>::type
ceil_pow2(T value)
{
	value = value - 1;
	value = value | (value >> 1);
	value = value | (value >> 2);
	value = value | (value >> 4);
	value = value | (value >> 8);
	value = value | (value >> 16);
	
	return value + 1;
}

#ifndef BOOST_HAS_INT64_T
template <typename T>
typename enable_if_c<is_integral<T>::value && sizeof(T) == 8, T>::type
ceil_pow2(T value)
{
	value = value - 1;
	value = value | (value >> 1);
	value = value | (value >> 2);
	value = value | (value >> 4);
	value = value | (value >> 8);
	value = value | (value >> 16);
	value = value | (value >> 32);
	
	return value + 1;
}
#endif
	
template <typename T>
typename enable_if_c<is_integral<T>::value && sizeof(T) == 1, T>::type
floor_pow2(T value)
{
	value = value | (value >> 1);
	value = value | (value >> 2);
	value = value | (value >> 4);
	
	return value - (value >> 1);
}
	
template <typename T>
typename enable_if_c<is_integral<T>::value && sizeof(T) == 2, T>::type
floor_pow2(T value)
{
	value = value | (value >> 1);
	value = value | (value >> 2);
	value = value | (value >> 4);
	value = value | (value >> 8);
	
	return value - (value >> 1);
}
	
template <typename T>
typename enable_if_c<is_integral<T>::value && sizeof(T) == 4, T>::type
floor_pow2(T value)
{
	value = value | (value >> 1);
	value = value | (value >> 2);
	value = value | (value >> 4);
	value = value | (value >> 8);
	value = value | (value >> 16);
	
	return value - (value >> 1);
}

#ifndef BOOST_HAS_INT64_T
template <typename T>
typename enable_if_c<is_integral<T>::value && sizeof(T) == 8, T>::type
floor_pow2(T value)
{
	value = value | (value >> 1);
	value = value | (value >> 2);
	value = value | (value >> 4);
	value = value | (value >> 8);
	value = value | (value >> 16);
	value = value | (value >> 32);
	
	return value - (value >> 1);
}
#endif
	
}

#endif
