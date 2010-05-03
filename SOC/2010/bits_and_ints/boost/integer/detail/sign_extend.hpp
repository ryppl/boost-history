//  Boost integer/sign_extend.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_SIGN_EXTEND
#define BOOST_SIGN_EXTEND

#include <boost/cstdint.hpp> // for uintmax_t
#include <boost/assert.hpp>

namespace boost 
{
	
typedef intmax_t Raw;

// Extend data represented in 'b' bits to sizeof(T)*8 bits
template <typename IntegralType>
inline Raw sign_extend(IntegralType data, unsigned b)
{
	data = data & ((Raw(1) << b) - 1);
	Raw const m = (Raw(1) << (b - 1));
	return (data ^ m) - m;
}

// Compile-time version of sign_extend
template<typename IntegralType, Raw raw, unsigned B>
struct static_sign_extend 
{
	typedef IntegralType T;
	BOOST_STATIC_CONSTANT(Raw, 
		value = ((raw & ((T(1) << B) - 1)) ^ (T(1) << (B - 1))) - (T(1) << (B - 1)) ); 
}; // boost::static_sign_extend
	

}

#endif