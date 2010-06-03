//  Boost integer/utils.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_INTEGER_UTILS_INCLUDED
#define BOOST_INTEGER_UTILS_INCLUDED

#include <boost/cstdint.hpp>

/*
 *	Some utilities to handle integers.
 */
namespace boost
{
	
// Sets the bit `pos' in data
template <typename T, T data, unsigned char pos>
struct set_bit
{
	BOOST_STATIC_CONSTANT(T, value = data | (T(1) << pos));
}; // set_bit
	
// Clear the bit `pos' in data
template <typename T, T data, unsigned char pos>
struct clear_bit
{
	BOOST_STATIC_CONSTANT(T, value = data & ~(T(1) << pos));
}; // clear_bit

// If the bit `pos' is 1 then it will be 0 if not the bit will be 1
template <typename T, T data, unsigned char pos>
struct flip_bit
{
	BOOST_STATIC_CONSTANT(T, value = data ^ (T(1) << pos));
}; // flip_bit

// Test if the bit in `pos' positon is set or not
template <typename T, T data, unsigned char pos>
struct is_bit_set
{
	BOOST_STATIC_CONSTANT(T, value = ((data >> pos) & T(1)) != T(0));
}; // is_bit_set

// Changes the value of bit in `pos' position
template <typename T, T data, unsigned char pos, bool state>
struct modify_bit
{
	BOOST_STATIC_CONSTANT(T, value = ((data & ~(T(1) << pos)) | (-state & (T(1) << pos))));
}; // modify_bit	
	
} // boost

#endif