//  Boost integer/round_power_2.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 


#ifndef BOOST_POWER_2_ROUND
#define BOOST_POWER_2_ROUND

/*
 *	The functions declared in this file rounds up or down an integral to the next power of 2.
 *
 *	The functions `ceil_pow2()' rounds up and the functions
 *		`floor_pow2()' rounds down.
 *
 *	Usage:
 *	
 *	T x = ceil_pow2(y);  // rounds down
 *  T z = floor_pow2(w); // rounds up
 */

namespace boost {
	
uint8_t ceil_pow2(uint8_t value)
{
	value = value - 1;
	value = value | (value >> 1);
	value = value | (value >> 2);
	value = value | (value >> 4);

	return value + 1;
}

uint16_t ceil_pow2(uint16_t value)
{
	value = value - 1;
	value = value | (value >> 1);
	value = value | (value >> 2);
	value = value | (value >> 4);
	value = value | (value >> 8);
	
	return value + 1;
}
	
uint32_t ceil_pow2(uint32_t value)
{
	value = value - 1;
	value = value | (value >> 1);
	value = value | (value >> 2);
	value = value | (value >> 4);
	value = value | (value >> 8);
	value = value | (value >> 16);
	
	return value + 1;
}

#ifndef BOOST_HAS_INT64_T
uint64_t ceil_pow2(uint64_t value)
{
	value = value - 1;
	value = value | (value >> 1);
	value = value | (value >> 2);
	value = value | (value >> 4);
	value = value | (value >> 8);
	value = value | (value >> 16);
	value = value | (value >> 32);
	
	return value + 1;
}
#endif
	
uint8_t floor_pow2(uint8_t value)
{
	value = value | (value >> 1);
	value = value | (value >> 2);
	value = value | (value >> 4);
	
	return value - (value >> 1);
}
	
uint16_t floor_pow2(uint16_t value)
{
	value = value | (value >> 1);
	value = value | (value >> 2);
	value = value | (value >> 4);
	value = value | (value >> 8);
	
	return value - (value >> 1);
}
	
uint32_t floor_pow2(uint32_t value)
{
	value = value | (value >> 1);
	value = value | (value >> 2);
	value = value | (value >> 4);
	value = value | (value >> 8);
	value = value | (value >> 16);
	
	return value - (value >> 1);
}

#ifndef BOOST_HAS_INT64_T
uint64_t floor_pow2(uint64_t value)
{
	value = value | (value >> 1);
	value = value | (value >> 2);
	value = value | (value >> 4);
	value = value | (value >> 8);
	value = value | (value >> 16);
	value = value | (value >> 32);
	
	return value - (value >> 1);
}
#endif
	
}

#endif
