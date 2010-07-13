//  Boost integer/static_isign.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_STATIC_ISIGN_INCLUDED
#define BOOST_STATIC_ISIGN_INCLUDED

#include <boost/static_assert.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/integer/is_integral_constant.hpp>

namespace boost {
	
/*
 *	MPL and static version of isign (transfer of sign function)
 */
	

namespace mpl {

/*
 *	This metafunction requires IC1 and IC2 to be mpl::integral_c<> types
 */
template <typename IC1, typename IC2>
struct isign : integral_c<typename IC1::value_type, 
	((IC1::value ^ ((IC1::value ^ IC2::value) >> (sizeof(typename IC1::value_type) * 8 - 1)))
		- ((IC1::value ^ IC2::value) >> (sizeof(typename IC1::value_type) * 8 - 1)))
>
{
	BOOST_STATIC_ASSERT((is_integral_constant<IC1>::value));
	BOOST_STATIC_ASSERT((is_integral_constant<IC2>::value));
};

} // mpl

/*
 *	This metafunction requires T to be an integral type
 */
template <typename T, T Value1, T Value2>
struct static_isign : mpl::isign< 
	mpl::integral_c<T, Value1>, 
	mpl::integral_c<T, Value2>
>
{
	BOOST_STATIC_ASSERT((is_integral<T>::value));
};
	
} // boost

#endif
