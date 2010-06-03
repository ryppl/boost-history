//  Boost integer/sign_extend.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_SIGN_EXTEND_INCLUDED
#define BOOST_SIGN_EXTEND_INCLUDED

#include <boost/type_traits.hpp> // is_integral
#include <boost/utility/enable_if.hpp> // enable_if

namespace boost 
{

//	Extend data represented in `bits' bits to 
//		sizeof(Type) * 8 bits
template <typename Type>
inline typename enable_if<is_integral<Type>, Type>::type
sign_extend(Type data, std::size_t bits)
{
	data = data & ((Type(1) << bits) - 1);
	Type const mask = (Type(1) << (bits - 1));
	return (data ^ mask) - mask;
}
	
} // boost

#endif
