//  Boost integer/detail/static_sign_extend.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_STATIC_SIGN_EXTEND_INCLUDED
#define BOOST_STATIC_SIGN_EXTEND_INCLUDED

#include <boost/cstdint.hpp> // for intmax_t

namespace boost 
{

// Compile-time version of sign_extend
template<typename IntegralType, Raw raw, unsigned B>
struct static_sign_extend 
{
	typedef IntegralType T;
private:
    BOOST_STATIC_CONSTANT(T, shift = (T(1) << (B - 1)));
	BOOST_STATIC_CONSTANT(T, m = ((T(1) << B) - 1));
public:
	BOOST_STATIC_CONSTANT(Raw, value = ((raw & m) ^ shift) - shift); 
}; // boost::static_sign_extend

} // boost

#endif