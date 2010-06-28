//  Boost integer/static_bit_reversal.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_STATIC_BIT_REVERSAL_INCLUDED
#define BOOST_STATIC_BIT_REVERSAL_INCLUDED

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/integer/detail/static_bit_reversal.hpp>
#include <boost/mpl/integral_c.hpp>

/* 
 *	Reverses the bits in data in compile-time
 *	For example if binary representation of data is:
 *		00000000000010001010101000001111
 *	static_bit_reversal<type, data>::value is:
 *		11110000010101010000000000000000
 */

namespace boost 
{

//	If T is an integral type, static_bit_reversal<T, data>::value will
//		be `data' with the bits reversed
template <typename T, T data, class Enable = typename enable_if< is_integral<T> >::type>
struct static_bit_reversal : mpl::integral_c<T,
	integer_detail::static_bit_reversal_impl<
		T, 
		data, 
		sizeof(T) * 8
	>::value
>
{}; // struct static_bit_reversal<>
	
namespace mpl {

/*
 * Boost MPL compatible metafunctions
 */
	
template <typename IC>
struct bit_reversal : integral_c<typename IC::value_type,
	integer_detail::static_bit_reversal_impl<
		typename IC::value_type, 
		IC::value, sizeof(typename IC::value_type) * 8
	>::value
>
{}; // struct bit_reversal


} // namespace mpl

} // namespace boost

#endif	