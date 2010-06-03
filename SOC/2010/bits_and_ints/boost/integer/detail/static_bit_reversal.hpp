//  Boost integer/detail/static_bit_reversal.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_DETAIL_STATIC_BIT_REVERSAL_INCLUDED
#define BOOST_DETAIL_STATIC_BIT_REVERSAL_INCLUDED

#include <boost/type_traits/integral_constant.hpp>
#include <boost/integer/bit_utils.hpp>

namespace boost {
namespace integer_detail {

//	Recursion implementation
template <typename T, T data, std::size_t shift>
struct static_bit_reversal_impl : 
	public integral_constant<T, 
		((test_bit<T, data, (sizeof(T) * 8 - shift)>::value << (shift - 1)) 
		 + static_bit_reversal_impl<T, data, shift - 1>::value)
	>
{};

//	Base case
template <typename T, T data>
struct static_bit_reversal_impl<T, data, 0> : 
	integral_constant<T, T(0)> {};


} // integer_detail
} // boost

#endif