//  Boost integer/clear_least_bit_set.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_CLEAR_LEAST_BIT_SET
#define BOOST_CLEAR_LEAST_BIT_SET

/*
 *	Clears last 1 bit in `value'.
 *	For example:
 *
 *	int value = 0xFA; // 0000 0000 1111 1010
 *  int x = clear_least_bit_set(value); 
 *	So x will be 0xF8 (0000 0000 1111 1000)
 */

namespace boost {

template <typename T>
T clear_least_bit_set(T value)
{
	return (value & (value - 1));
} //clear_least_bit_set
	
} //boost

#endif