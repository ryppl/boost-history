//  Boost integer/count_trailing_zeros.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_COUNT_TRAILING_ZEROS_INCLUDED
#define BOOST_COUNT_TRAILING_ZEROS_INCLUDED

#include <boost/integer/pop_count.hpp>
#include <boost/cstdint.hpp>

/*
 *	count_trailing_zeros counts the number of consecutive 0's 
 *		from the least significant bit of an integral value.
 *
 *	For example:
 *	
 *                                   trailing zeros: vvv vvvv
 *	unsigned value = 0xF080; // 0000 0000 1111 0000 1000 0000
 *  int x = count_trailing_zeros(value); // x = 7 
 * 
 */

namespace boost {

int count_trailing_zeros(uintmax_t value)
{
	return pop_count(~value & (value - 1));
} // count_trailing_zeros
	
} // boost

#endif