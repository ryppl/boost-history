//  Boost integer/isqrt.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_ISQRT_INCLUDED
#define BOOST_ISQRT_INCLUDED

#include <boost/cstdint.hpp>

namespace boost {
	
/*	
 *	These functions calculates the floor of square 
 *		root of integral values.
 */

int isqrt(uint8_t value)
{
	uint8_t mask, result, aux;
	
	mask = 0x40;
	result = 0;
	
	while (mask != 0) {
		aux = result | mask;
		result >>= 1;
		
		if (value >= aux) {
			value -= aux;
			result |= mask;
		}
		
		mask >>= 2;
	}
	
	return result;	
}
	
int isqrt(uint16_t value)
{
	uint16_t mask, result, aux;
	
	mask = 0x4000;
	result = 0;
	
	while (mask != 0) {
		aux = result | mask;
		result >>= 1;
		
		if (value >= aux) {
			value -= aux;
			result |= mask;
		}
		
		mask >>= 2;
	}
	
	return result;	
}

int isqrt(uint32_t value)
{
	uint32_t mask, result, aux;
	
	mask = 0x40000000;
	result = 0;
	
	while (mask != 0) {
		aux = result | mask;
		result >>= 1;
		
		if (value >= aux) {
			value -= aux;
			result |= mask;
		}
		
		mask >>= 2;
	}
	
	return result;	
}

/*#ifndef BOOST_HAS_NO_INT64_T
int64_t isqrt(uint64_t value)
{
	uint64_t mask, result, aux;
	
	mask = 0x4000000000000000ULL;
	result = 0;
	
	while (mask != 0) {
		aux = result | mask;
		result >>= 1;
		
		if (value >= aux) {
			value -= aux;
			result |= mask;
		}
		
		mask >>= 2;
	}
	
	return result;	
}
#endif*/

}

#endif
