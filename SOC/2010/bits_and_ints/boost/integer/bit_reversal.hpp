//  Boost integer/bit_reversal.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_BIT_REVERSAL_INCLUDED
#define BOOST_BIT_REVERSAL_INCLUDED

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/cstdint.hpp>

namespace boost 
{
	
/* 
 *	Reverses the bits in data
 *	For example if binary representation of data is:
 *		00000000000010001010101000001111
 *	bit_reversal(data) is:
 *		11110000010101010000000000000000
 */

template <typename T>
inline typename enable_if_c<is_integral<T>::type::value 
	&& (sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8), T>::type
bit_reversal(T data)
{
	
	static const uintmax_t mask[4][5] = {
		{0x55, 0x33, 0xFF, 0xFF, 0xFF},
		{0x5555, 0x3333, 0x0F0F, 0xFFFF, 0xFFFF},
		{0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF, 0xFFFFFFFF},
#ifndef BOOST_HAS_NO_INT64_T
		{0x5555555555555555, 0x3333333333333333, 0x0F0F0F0F0F0F0F0F, 0x00FF00FF00FF00FF, 0x0000FFFF0000FFFF}
#else
		{0x0, 0x0, 0x0, 0x0, 0x0}
#endif
	};
	
	static const std::size_t shift[4][6] = {
		{1, 2, 4, 0, 0, 0},
		{1, 2, 4, 8, 0, 0},
		{1, 2, 4, 8, 16, 0},
		{1, 2, 4, 8, 16, 32}
	};
	
	const int index = (sizeof(T) / 2) - (sizeof(T) / 8);
	
	data = ((data >> shift[index][0]) & mask[index][0]) | ((data & mask[index][0]) << shift[index][0]); 
	data = ((data >> shift[index][1]) & mask[index][1]) | ((data & mask[index][1]) << shift[index][1]); 
	data = ((data >> shift[index][2]) & mask[index][2]) | ((data & mask[index][2]) << shift[index][2]); 
	data = ((data >> shift[index][3]) & mask[index][3]) | ((data & mask[index][3]) << shift[index][3]); 
	data = ((data >> shift[index][4]) & mask[index][4]) | ((data & mask[index][4]) << shift[index][4]);
#ifndef BOOST_HAS_NO_INT64_T
	data = (data >> shift[index][5] | data << shift[index][5]);
#endif
	return data;
}

} // boost

#endif