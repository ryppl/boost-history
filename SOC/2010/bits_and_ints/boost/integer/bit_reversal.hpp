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

namespace boost 
{
	
/* 
 *	Reverses the bits in data
 *	For example if binary representation of data is:
 *		00000000000010001010101000001111
 *	bit_reversal(data) is:
 *		11110000010101010000000000000000
 */


#ifndef BOOST_NO_INT64_T	
// 64-bit version
template <typename T>
inline typename enable_if_c<is_integral<T>::type::value && sizeof(T) == 8, T>::type
bit_reversal(T data)
{
	// swap odd and even bits
	data = ((data >> 1) & 0x5555555555555555) | ((data & 0x5555555555555555) << 1); 
	// swap consecutive pairs
	data = ((data >> 2) & 0x3333333333333333) | ((data & 0x3333333333333333) << 2); 
	// swap nibbles 
	data = ((data >> 4) & 0x0F0F0F0F0F0F0F0F) | ((data & 0x0F0F0F0F0F0F0F0F) << 4); 
	// swap bytes
	data = ((data >> 8) & 0x00FF00FF00FF00FF) | ((data & 0x00FF00FF00FF00FF) << 8); 
	// swap 2-byte long pairs
	data = ((data >> 16) & 0x0000FFFF0000FFFF) | ((data & 0x0000FFFF0000FFFF) << 16);
	// swap the halves
	data = (data >> 32 | data << 32);
	
	return data;
}
#endif
	
// 32-bit version
template <typename T>
inline typename enable_if_c<is_integral<T>::type::value && sizeof(T) == 4, T>::type
bit_reversal(T data)
{
	// swap odd and even bits
	data = ((data >> 1) & 0x55555555) | ((data & 0x55555555) << 1);
	// swap consecutive pairs
	data = ((data >> 2) & 0x33333333) | ((data & 0x33333333) << 2);
	// swap nibbles 
	data = ((data >> 4) & 0x0F0F0F0F) | ((data & 0x0F0F0F0F) << 4);
	// swap bytes
	data = ((data >> 8) & 0x00FF00FF) | ((data & 0x00FF00FF) << 8);
	// swap 2-byte long pairs
	data = (data >> 16) | (data << 16);
	
	return data;
}
	
// 16-bit version
template <typename T>
inline typename enable_if_c<is_integral<T>::type::value && sizeof(T) == 2, T>::type
bit_reversal(T data)
{
	// swap odd and even bits
	data = ((data >> 1) & 0x5555) | ((data & 0x5555) << 1);
	// swap consecutive pairs
	data = ((data >> 2) & 0x3333) | ((data & 0x3333) << 2);
	// swap nibbles 
	data = ((data >> 4) & 0x0F0F) | ((data & 0x0F0F) << 4);
	// swap bytes
	data = (data >> 8) | (data << 8);
	
	return data;
}
	
// 8-bit version
template <typename T>
inline typename enable_if_c<is_integral<T>::type::value && sizeof(T) == 1, T>::type
bit_reversal(T data)
{
	// swap odd and even bits
	data = ((data >> 1) & 0x55) | ((data & 0x55) << 1);
	// swap consecutive pairs
	data = ((data >> 2) & 0x33) | ((data & 0x33) << 2);
	// swap nibbles 
	data = (data >> 4) | (data << 4);
	
	return data;
}

} // boost

#endif