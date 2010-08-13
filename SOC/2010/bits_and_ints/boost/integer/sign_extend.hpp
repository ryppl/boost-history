//  Boost integer/sign_extend.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_SIGN_EXTEND_INCLUDED
#define BOOST_SIGN_EXTEND_INCLUDED

#include <boost/assert.hpp>
#include <boost/type_traits/is_integral.hpp>

namespace boost {

//	Extend data represented in `bits' bits to 
//		sizeof(Type) * 8 bits
template <typename T>
inline T sign_extend(T data, std::size_t bits)
{
	BOOST_ASSERT((bits < sizeof(T) * 8));
	
	data = data & ((T(1) << bits) - 1);
	T const mask = (T(1) << (bits - 1));
	return (data ^ mask) - mask;
}
	
} // boost

#endif
