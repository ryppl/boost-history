//  Boost integer/isign.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_ISIGN_INCLUDED
#define BOOST_ISIGN_INCLUDED

namespace boost {
	
/*
 *	`isign' or transfer of sign function is defined by:
 *	
 *	isign(x, y) =
 *		abs(x), if y is greater than or equal 0,
 *		-abs(x), if y is less than 0.
 */
	
template <typename T>
T isign(T x, T y)
{
	const T tmp = (x ^ y) >> (sizeof(T) * 8 - 1);
	return (x ^ tmp) - tmp;
}
	
} // boost

#endif
