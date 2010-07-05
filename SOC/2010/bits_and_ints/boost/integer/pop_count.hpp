//  Boost integer/pop_count.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_POP_COUNT_INCLUDED
#define BOOST_POP_COUNT_INCLUDED

#include <boost/cstdint.hpp>
#include <boost/integer/detail/pop_count.hpp>

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
	using integer_detail::pop_count_mask;
	value = (value & pop_count_mask[0]) + ((value >> 1) & pop_count_mask[0]);
	value = (value & pop_count_mask[1]) + ((value >> 2) & pop_count_mask[1]);
	value = (value & pop_count_mask[2]) + ((value >> 4) & pop_count_mask[2]);
	value = (value & pop_count_mask[3]) + ((value >> 8) & pop_count_mask[3]);
	value = (value & pop_count_mask[4]) + ((value >> 16) & pop_count_mask[4]);
#ifndef BOOST_NO_INT64_T
	value = (value & pop_count_mask[5]) + ((value >> 32) & pop_count_mask[5]);
#endif
	
	return int(value);
} // pop_count

} // boost

#endif