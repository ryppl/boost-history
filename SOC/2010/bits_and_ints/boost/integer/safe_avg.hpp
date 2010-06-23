//  Boost integer/safe_avg.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

/*
 *	Given two integer values x and y, the (floor of the) average normally
 *		would be computed by (x+y)/2; unfortunately, 
 *		this can yield incorrect results due to overflow.
 *	
 *	`safe_avg(x, y)' ensures that no overflow will happen even if (x+y) 
 *		overflows the range of T.
 */

#ifndef BOOST_SAFE_AVG_INCLUDED
#define BOOST_SAFE_AVG_INCLUDED

namespace boost {

template <typename T>
inline T safe_avg(T x, T y)
{
	return (x & y) + ((x ^ y) >> 1);
} // safe_avg
	
} // boost

#endif