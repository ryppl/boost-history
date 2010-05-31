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

// Base container
template <typename T, T val>
struct static_bit_reversal_base
{
	BOOST_STATIC_CONSTANT(T, value = val);
};

//	@TODO: shift template parameter 
//		must not be changeable by user
template <typename T, T val, std::size_t shift = sizeof(T) * 8>
struct static_bit_reversal : 
	public static_bit_reversal_base<T, 
		((is_bit_set<T, val, (sizeof(T) * 8 - shift)>::value << (shift - 1)) 
		+ static_bit_reversal<T, val, shift - 1>::value)
	>
{};

//	Base case
template <typename T, T val>
struct static_bit_reversal<T, val, 0> : 
	static_bit_reversal_base<T, T(0)> {};

	
} // boost

#endif