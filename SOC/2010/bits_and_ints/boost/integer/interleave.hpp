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
#include <boost/integer/morton.hpp>
#include <utility>

/*
 *	Bit interleaving functions.
 *
 *	Interleave the bits of two integers in one morton<> type.
 *	For example:
 *		uint8_t a = 5;  // 0000 0101
 *		uint8_t b = 10; // 0000 1010
 *		morton<16> z = interleave(a, b); // z = 102
 *		
 *		a       = 0 0 0 0 0 1 0 1
 *		b       =  0 0 0 0 1 0 1 0
 *		z.value = 0000000001100110
 */

namespace boost 
{
	
/*
 *	Interleave two integrals of 8, 16 or 32 bits and returns
 *		one morton<S> with S equal to the double of the size of bits of `x' or `y'.
 */
template <typename T>
inline typename enable_if_c<is_integral<T>::type::value 
	&& (sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4) , morton<sizeof(T) * 16> >::type
interleave(T x, T y)
{
	static const uintmax_t
	interleave_mask[3][5] = {
		{0x5555, 0x3333, 0x0F0F, 0xFFFF, 0xFFFF},
		{0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF, 0xFFFFFFFF},
#ifndef BOOST_NO_INT64_T
		{0x5555555555555555, 0x3333333333333333, 0x0F0F0F0F0F0F0F0F, 0x00FF00FF00FF00FF, 0x0000FFFF0000FFF }
#else
		{0x0, 0x0, 0x0, 0x0, 0x0}
#endif
	};
	
	static const std::size_t
	interleave_shift[3][5] = {
		{0, 0, 4, 2, 1},
		{0, 8, 4, 2, 1},
		{16, 8, 4, 2, 1}
	};
	
	const std::size_t bytes = sizeof(T);
	typedef typename integer_detail::morton_type<bytes * 16>::type value_type;
	
	value_type a(x), b(y);
	
	a = (a | (a << interleave_shift[bytes/2][0])) & value_type(interleave_mask[bytes/2][4]);
	
	// puts the byte 2 in the byte 4 and the byte 3 in the byte 5
	a = (a | (a << interleave_shift[bytes/2][1])) & value_type(interleave_mask[bytes/2][3]);
	
	// now the original bytes of x are in alternate bytes
	a = (a | (a << interleave_shift[bytes/2][2])) & value_type(interleave_mask[bytes/2][2]);
	
	// now the original bits of x are in the lowest 2 bits of the nibbles
	a = (a | (a << interleave_shift[bytes/2][3])) & value_type(interleave_mask[bytes/2][1]);
	
	// now the original bits are in the even bits
	a = (a | (a << interleave_shift[bytes/2][4])) & value_type(interleave_mask[bytes/2][0]);
	
	// do the same with b
	b = (b | (b << interleave_shift[bytes/2][0])) & value_type(interleave_mask[bytes/2][4]);
	b = (b | (b << interleave_shift[bytes/2][1])) & value_type(interleave_mask[bytes/2][3]);
	b = (b | (b << interleave_shift[bytes/2][2])) & value_type(interleave_mask[bytes/2][2]);
	b = (b | (b << interleave_shift[bytes/2][3])) & value_type(interleave_mask[bytes/2][1]);
	b = (b | (b << interleave_shift[bytes/2][4])) & value_type(interleave_mask[bytes/2][0]);
	
	// now the original bits are in the odd bits
	b <<= 1;

	// the result is the union of a and b bits
	return (a | b);
	
} // interleave
	
/*
 *	This function "uninterleaves" a morton number into two morton number with
 *		half size.
 *	
 *	The parameter number must be an morton<S> with S equal 16, 32 or 64.
 */
template <std::size_t bits>
inline typename enable_if_c<bits == 16 || bits == 32 || bits == 64, 
	std::pair<morton<bits/2>, morton<bits/2> > >::type 
uninterleave(const morton<bits>& number)
{
	morton<bits/2> a, b;
	
	for (int i = 0; i < bits; i += 2) {
		a.value |= ((number.value & (1 << i))) >> (i >> 1);
		b.value |= ((number.value & (1 << (i + 1)))) >> ((i >> 1) + 1);
	}
	
	return make_pair(a, b);
} // uninterleave

} // boost
#endif