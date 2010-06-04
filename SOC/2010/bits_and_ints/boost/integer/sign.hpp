//  Boost integer/sign.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <iostream>
#include <limits>

namespace boost
{

template <typename T>
inline typename enable_if<is_signed<T>, int>::type
sign(T data)
{
	const int shift = std::numeric_limits<T>::digits;
	
	// -1 if data is negative
	register const int is_negative = data >> shift;
	
	typename make_unsigned<T>::type negative = 
		static_cast<typename make_unsigned<T>::type>(-data);
	
	// 1 if data is positive
	register const int is_positive = negative >> shift;
	
	return is_negative | is_positive;
}

template <typename T>
inline typename enable_if<is_unsigned<T>, int>::type
sign(T data)
{
	return int(!!data);
}

} // boost