//  Boost integer/pop_count.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_POP_COUNT_INCLUDED
#define BOOST_POP_COUNT_INCLUDED

#include <boost/cstdint.hpp>

/*
 *	Population count (pop_count) counts the number of 1-bits in an
 *		integral value.
 *
 *	For example:
 *  
 *  uint8_t value = 0xAF; // value is 10101111
 *
 *	int x = pop_count(value); // x = 6
 *
 */

namespace boost {

int pop_count(uintmax_t value)
{
	static const uintmax_t mask[6] = {
#ifndef BOOST_NO_INT64_T
		0x5555555555555555, 0x3333333333333333, 0x0F0F0F0F0F0F0F0F, 
			0x00FF00FF00FF00FF, 0x0000FFFF0000FFFF, 0x00000000FFFFFFFF
#else
		0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF, 0x0000FFFF, 0x0
#endif
	};
	
	value = (value & mask[0]) + ((value >> 1) & mask[0]);
	value = (value & mask[1]) + ((value >> 2) & mask[1]);
	value = (value & mask[2]) + ((value >> 4) & mask[2]);
	value = (value & mask[3]) + ((value >> 8) & mask[3]);
	value = (value & mask[4]) + ((value >> 16) & mask[4]);
#ifndef BOOST_NO_INT64_T
	value = (value & mask[5]) + ((value >> 32) & mask[5]);
#endif
	
	return int(value);
} // pop_count

} // boost

#endif