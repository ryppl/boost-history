//  Boost integer/rrshift.hpp header file  ---------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_RRSHIFT_INCLUDED
#define BOOST_RRSHIFT_INCLUDED

#include <boost/integer/sign.hpp>
#include <boost/type_traits/make_unsigned.hpp>

namespace boost {
	
/*
 *	rrshift function perform a right shift rounding towards 0.
 */

template <typename T>
inline T rrshift(T value, unsigned shift)
{
	return sign(value) * (typename make_unsigned<T>::type(sign(value) * value) >> shift);
}
	
}

#endif