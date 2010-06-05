//  Boost integer/same_sign.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#include <boost/type_traits/is_integral.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/integer/bit_utils.hpp>
#include <limits>

namespace boost
{

/*
 *	same_sign(first, second) returns:
 *		- false: if the signs of first and second are different
 *		- true: if the signs are equal
 */
	
template <typename T>
inline typename enable_if<is_integral<T>, bool>::type
same_sign(T first, T second)
{
	T temp = first ^ second;
	temp = temp >> ((sizeof(T) * 8) - 1);
	temp = temp & T(1);
	
	return !temp;
}

/*
 *	Compile-time version of same_sign
 *
 *	static_same_sign<type, FIRST, SECOND>::value will be:
 *		- false: if the signs of FIRST and SECOND are different
 *		- true: if the signs are equal
 */
template <typename T, T first, T second, class Enable = typename enable_if<is_integral<T> >::type>
struct static_same_sign
{
private:
	static const int temp = (first ^ second) >> ((sizeof(T) * 8) - 1);
public:
	BOOST_STATIC_CONSTANT(bool, value = !(temp & 1));
};
	
}