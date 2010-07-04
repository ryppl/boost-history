//  Boost integer/same_sign.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost {

/*
 *	Compile-time version of same_sign.
 *		mpl::same_sign< integral_c<T, X>, integral_c<T, Y> >::value
 *		or static_same_sign<X, Y>::value will be:
 *			- false: if the signs of X and Y are different
 *			- true: if the signs are equal
 *
 *		Note that 0 is not considered a positive nor negative integral, so
 *			static_same_sign<0, 1>::value is *false*.
 */

namespace mpl {
	
/*
 *	MPL compatible static version of same_sign
 *		IC1 and IC2 types must be mpl::integral_c<> types.
 */
template <typename IC1, typename IC2>
struct same_sign : bool_<((!IC1::value && !IC2::value) 
	|| (IC1::value < 0 && IC2::value < 0) 
	|| (IC2::value > 0 && IC2::value > 0))
>
{};
	
}

/*
 *	static_same_sign<type, FIRST, SECOND>::value will be:
 *		- false: if the signs of FIRST and SECOND are different
 *		- true: if the signs are equal
 */
template <typename T, T first, T second, class Enable = typename enable_if<is_integral<T> >::type>
struct static_same_sign : mpl::same_sign< mpl::integral_c<T, first>, mpl::integral_c<T, second> >
{};

}