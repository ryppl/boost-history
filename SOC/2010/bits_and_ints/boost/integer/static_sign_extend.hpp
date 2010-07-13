//  Boost integer/static_sign_extend.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_STATIC_SIGN_EXTEND_INCLUDED
#define BOOST_STATIC_SIGN_EXTEND_INCLUDED

#include <boost/static_assert.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/integer/is_integral_constant.hpp>

/* 
 *	Extend `data' represented in `Bits' bits to sizeof(T) * 8
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

#ifdef BOOST_MSVC
#pragma warning ( disable : 4307 )
#endif

namespace boost {
	
namespace mpl {

/*
 *	Boost MPL compatible metafunction
 *	IC is a mpl::integral_c<> type
 */

template <typename IC, std::size_t Bits>
struct sign_extend : integral_c<typename IC::value_type,
	(((IC::value & (((typename IC::value_type(1)) << Bits) - 1)) 
	^ (typename IC::value_type(1) << (Bits - 1))) 
	- (typename IC::value_type(1) << (Bits - 1)))
>
{
	BOOST_STATIC_ASSERT((is_integral_constant<IC>::value));
	BOOST_STATIC_ASSERT((Bits > 0 && Bits < (sizeof(typename IC::value_type) * 8)));
};

}

// Compile-time version of sign_extend
template<typename T, T Value, std::size_t Bits>
struct static_sign_extend : mpl::sign_extend<mpl::integral_c<T, Value>, Bits>
{
	BOOST_STATIC_ASSERT((is_integral<T>::value));
	BOOST_STATIC_ASSERT((Bits > 0 && Bits < (sizeof(T) * 8)));
}; // boost::static_sign_extend

} // boost

#endif
