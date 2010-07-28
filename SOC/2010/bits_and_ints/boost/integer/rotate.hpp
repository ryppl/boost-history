//  Boost integer/rotate.hpp header file  ---------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_ROTATE_INCLUDED
#define BOOST_ROTATE_INCLUDED

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/make_unsigned.hpp>

namespace boost {

template <typename T>
inline typename enable_if<is_integral<T>, T>::type
rotate_left(T value, unsigned length)
{
	return (value << length) | (typename make_unsigned<T>::type(value) >> ((sizeof(T) << 3) - length));
}

template <typename T>
inline typename enable_if<is_integral<T>, T>::type
rotate_right(T value, unsigned length)
{
	return (typename make_unsigned<T>::type(value) >> length) | (value << ((sizeof(T) << 3) - length));
}
									
}
#endif