//  Boost integer/interleave.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_INTERLEAVE_INCLUDED
#define BOOST_INTERLEAVE_INCLUDED

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/cstdint.hpp>


/*
 *	Bit interleaving functions.
 *
 *	Interleave the bits of two integers in one integer.
 *	For example:
 *		a = 5;  // 0000 0101
 *		b = 10; // 0000 1010
 *		z = interleave(a, b); // z = 102
 *		
 *		a = 0 0 0 0 0 1 0 1
 *		b =  0 0 0 0 1 0 1 0
 *		z = 0000000001100110
 */

namespace boost 
{

/*
 *	Interleave two 16-bit integral types and returns the bits interleaved in an
 *		uint32_t.
 */
template <typename T>
inline typename enable_if_c<is_integral<T>::type::value && sizeof(T) == 2, uint32_t>::type
interleave(T x, T y)
{
	uint32_t a(x), b(y);
	
	// puts the byte 2 in the byte 4 and the byte 3 in the byte 5
	a = (a | (a << 8)) & 0x00FF00FF;
	
	// now the original bytes of x are in alternate bytes
	a = (a | (a << 4)) & 0x0F0F0F0F;
	
	// now the original bits of x are in the lowest 2 bits of the nibbles
	a = (a | (a << 2)) & 0x33333333;
	
	// now the original bits are in the even bits
	a = (a | (a << 1)) & 0x55555555;
	
	// do the same with b
	b = (b | (b << 8)) & 0x00FF00FF;
	b = (b | (b << 4)) & 0x0F0F0F0F;
	b = (b | (b << 2)) & 0x33333333;
	b = (b | (b << 1)) & 0x55555555;
	
	// now the original bits are in the odd bits
	b <<= 1;

	// the result is the union of a and b bits
	return (a | b);
}

/*
 *	Interleave two 8-bit integral types and returns the bits interleaved in an
 *		uint16_t.
 */
template <typename T>
inline typename enable_if_c<is_integral<T>::type::value && sizeof(T) == 1, uint16_t>::type
interleave(T x, T y)
{
	uint16_t a(x), b(y);
	
	// now the original bytes are in alternate bytes
	a = (a | (a << 4)) & 0x0F0F;
	
	// now the original bits are in the lowest 2 bits of the nibbles
	a = (a | (a << 2)) & 0x3333;
	
	// now the original bits are in the even bits
	a = (a | (a << 1)) & 0x5555;
	
	// do the same with b
	b = (b | (b << 4)) & 0x0F0F;
	b = (b | (b << 2)) & 0x3333;
	b = (b | (b << 1)) & 0x5555;
	
	// now the original bits are inthe odd bits
	b <<= 1;
	
	// the result is the union of a and b bits
	return (a | b);
}

}
#endif