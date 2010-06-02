//  Boost integer/detail/static_bit_reversal.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_STATIC_BIT_REVERSAL_INCLUDED
#define BOOST_STATIC_BIT_REVERSAL_INCLUDED

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include "utils.hpp" // is_bit_set<>

/* 
 *	Reverses the bits in data in compile-time
 *	For example if binary representation of data is:
 *		00000000000010001010101000001111
 *	static_bit_reversal<type, data>::value is:
 *		11110000010101010000000000000000
 */

namespace boost 
{

namespace bits_and_ints_private {

// Base container
template <typename T, T data>
struct static_bit_reversal_base
{
	BOOST_STATIC_CONSTANT(T, value = data);
};

//	Recursion implementation
template <typename T, T data, std::size_t shift>
struct static_bit_reversal_impl : 
	public static_bit_reversal_base<T, 
		((is_bit_set<T, data, (sizeof(T) * 8 - shift)>::value << (shift - 1)) 
		 + static_bit_reversal_impl<T, data, shift - 1>::value)
	>
{};

//	Base case
template <typename T, T data>
struct static_bit_reversal_impl<T, data, 0> : 
	static_bit_reversal_base<T, T(0)> {};

} // bits_and_ints_private

	
//	If T is not an integral type, static_bit_reversal<T, data>::value call
//		will result an error.
template <typename T, T data, class Enable = void>
struct static_bit_reversal {};

//	If T is an integral type, static_bit_reversal<T, data>::value will
//		be `data' with the bits reversed
template <typename T, T data>
struct static_bit_reversal<T, data, typename enable_if<is_integral<T> >::type> { 
	BOOST_STATIC_CONSTANT(T, value = 
		(bits_and_ints_private::static_bit_reversal_impl<T, data, sizeof(T) * 8>::value)
	);
};

} // boost

#endif	