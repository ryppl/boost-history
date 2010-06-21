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
#include <boost/integer.hpp>
#include <utility>

/*
 *	Bit [un]interleaving functions.
 *
 *	interleave function interleaves the bits of two integrals in one integral
 *		with the double of the size.
 *	For example:
 *		uint8_t  a = 5;  // 0000 0101
 *		uint8_t  b = 10; // 0000 1010
 *		uint16_t z = interleave(a, b); // z = 102
 *		
 *		a = 0 0 0 0 0 1 0 1
 *		b =  0 0 0 0 1 0 1 0
 *		z = 0000000001100110
 *
 *	uninterleave function do the reverse step:
 *  pair< uint8_t, uint8_t > y = uninterleave(z);
 *	
 *	y.first  ==  a == 5
 *	y.second ==  b == 10
 */

namespace boost 
{
	
/*
 *	Interleave two unsigned integrals of 8, 16 or 32 bits and returns
 *		one unsigned integral with 16, 32 or 64 bits.
 */
template <int Bits>
inline typename enable_if_c<Bits == 8 || Bits == 16 || Bits == 32, typename uint_t<Bits * 2>::exact>::type 
interleave(typename uint_t<Bits>::exact x, typename uint_t<Bits>::exact y)
{
	// mask[0] - masks for 8-bit parameters
	//     [1] - 16-bit
	//     [2] - 32-bit
	static const uintmax_t mask[3][5] = {
		{0x5555, 0x3333, 0x0F0F, 0xFFFF, 0xFFFF},
		{0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF, 0xFFFFFFFF},
#ifndef BOOST_NO_INT64_T
		{0x5555555555555555, 0x3333333333333333, 0x0F0F0F0F0F0F0F0F, 0x00FF00FF00FF00FF, 0x0000FFFF0000FFF }
#else
		{0x0, 0x0, 0x0, 0x0, 0x0}
#endif
	};
	
	// shift[0] - shift sizes for 8-bit parameters
	//      [1] - 16-bit
	//      [2] - 32-bit
	static const std::size_t shift[3][5] = {
		{0, 0, 4, 2, 1},
		{0, 8, 4, 2, 1},
		{16, 8, 4, 2, 1}
	};
	
	// Index for access above arrays
	const int index = Bits / 16;
	
	// Return type is 2 times greater then the size of the parameters
	typedef typename uint_t<Bits * 2>::exact  return_type;
	
	return_type a(x), b(y);
	
	a = (a | (a << shift[index][0])) & return_type(mask[index][4]);
	
	// puts the byte 2 in the byte 4 and the byte 3 in the byte 5
	a = (a | (a << shift[index][1])) & return_type(mask[index][3]);
	
	// now the original bytes of x are in alternate bytes
	a = (a | (a << shift[index][2])) & return_type(mask[index][2]);
	
	// now the original bits of x are in the lowest 2 bits of the nibbles
	a = (a | (a << shift[index][3])) & return_type(mask[index][1]);
	
	// now the original bits are in the even bits
	a = (a | (a << shift[index][4])) & return_type(mask[index][0]);
	
	// do the same with b
	b = (b | (b << shift[index][0])) & return_type(mask[index][4]);
	b = (b | (b << shift[index][1])) & return_type(mask[index][3]);
	b = (b | (b << shift[index][2])) & return_type(mask[index][2]);
	b = (b | (b << shift[index][3])) & return_type(mask[index][1]);
	b = (b | (b << shift[index][4])) & return_type(mask[index][0]);
	
	// now the original bits are in the odd bits
	b <<= 1;

	// the result is the union of a and b bits
	return (a | b);
	
} // interleave
	
/*
 *	This function "uninterleaves" an unsigned integral into unsigned integrals
 *		with the half size.
 *	
 *	The parameter number must be unsigned integral with 16, 32 or 64 bits.
 */
template <int Bits>
inline typename enable_if_c<Bits == 16 || Bits == 32 || Bits == 64, 
	std::pair<typename uint_t<Bits/2>::exact, typename uint_t<Bits/2>::exact> >::type 
uninterleave(typename uint_t<Bits>::exact number)
{
	typename uint_t<Bits/2>::exact a, b;
	
	for (int i = 0; i < Bits; i += 2) {
		a |= ((number & (1 << i))) >> (i >> 1);
		b |= ((number & (1 << (i + 1)))) >> ((i >> 1) + 1);
	}
	
	return make_pair(a, b);
} // uninterleave

} // boost
#endif