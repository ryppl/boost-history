//  Boost integer/static_sign_extend.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_STATIC_SIGN_EXTEND_INCLUDED
#define BOOST_STATIC_SIGN_EXTEND_INCLUDED

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_integral.hpp>

namespace boost 
{

// Compile-time version of sign_extend
template<typename Type, Type data, std::size_t Bits, 
	class Enable = typename enable_if<is_integral<Type> >::type>
struct static_sign_extend 
{
private:
    BOOST_STATIC_CONSTANT(Type, shift = (Type(1) << (Bits - 1)));
	BOOST_STATIC_CONSTANT(Type, mask = ((Type(1) << Bits) - 1));
public:
	BOOST_STATIC_CONSTANT(Type, value = ((data & mask) ^ shift) - shift); 
}; // boost::static_sign_extend

} // boost

#endif