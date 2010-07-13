//  Boost integer/static_safe_avg.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_STATIC_SAFE_AVG_INCLUDED
#define BOOST_STATIC_SAFE_AVG_INCLUDED

#include <boost/static_assert.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/integer/is_integral_constant.hpp>

namespace boost {
	
/*
 *	Given two constant values x and y, the (floor of the) average normally
 *		would be computed by (x+y)/2 unfortunately, 
 *		this can yield incorrect results due to overflow.
 *	
 *	`static_safe_avg<T, x, y>::value' and mpl::safe_avg<IC1, IC2>::value ensures 
 *		that no overflow will happen even if (x+y) overflows the range of T.
 */
	
namespace mpl {

template <typename IC1, typename IC2>
struct safe_avg : integral_c<
	typename IC1::value_type, 
	((IC1::value & IC2::value) + ((IC1::value ^ IC2::value) >> 1))
>
{
	BOOST_STATIC_ASSERT((is_integral_constant<IC1>::value));
	BOOST_STATIC_ASSERT((is_integral_constant<IC2>::value));
};

}

template <typename T, T Value1, T Value2>
struct static_safe_avg : mpl::safe_avg< 
	mpl::integral_c<T, Value1>, 
	mpl::integral_c<T, Value2> 
>
{
	BOOST_STATIC_ASSERT((is_integral<T>::value));
};

} // boost

#endif
