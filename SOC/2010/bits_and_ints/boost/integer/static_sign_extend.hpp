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

/* 
 *	Extend `data' represented in `Bits' bits to sizeof(Type) * 8
 *		in compile-time.
 *
 *	For example if binary representation of `data' in 4 bits is:
 *		00000000000000000000000000001010
 *
 *  So, `data' is -6 in 4-bit 2-complement.
 *
 *	If type is a 32-bit integral,
 *		static_sign_extend<type, data, 4>::value in 2-complement is:
 *		11111111111111111111111111111010
 *
 *	Wich is -6 in 32-bit 2-complement.
 */

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