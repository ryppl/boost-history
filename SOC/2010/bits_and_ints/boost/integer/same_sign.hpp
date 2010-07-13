//  Boost integer/same_sign.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_integral.hpp>

namespace boost {

/*
 *	same_sign(first, second) returns:
 *		- false: if the signs of first and second are different
 *		- true: if the signs are equal
 *	
 *	Note that 0 is not considered a positive nor negative integral, so
 *			same_sign(0, 1) is *false*.
 */
	
template <typename T>
bool same_sign(T first, T second)
{
	BOOST_STATIC_ASSERT((is_integral<T>::value));
	
	T temp = first ^ second;
	temp = temp >> ((sizeof(T) * 8) - 1);
	temp = temp & T(1);
	
	return !temp;
}
	
}
