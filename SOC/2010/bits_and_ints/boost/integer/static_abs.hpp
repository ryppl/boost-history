//  Boost integer/static_abs.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  (C) Copyright Vicente J. Botet Escribá 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STATIC_ABS_INCLUDED
#define BOOST_STATIC_ABS_INCLUDED

#include <boost/utility/enable_if.hpp>
#include <boost/integer/is_integral_constant.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/make_unsigned.hpp>

namespace boost
{

/*
 *	This header defines mpl::abs<> and static_abs<> metafunctions.
 *	The mpl::abs<> version returns the absolute value of an mpl::integral_c<>
 *		and the static_abs version returns the absolute value from an integral value.
 */

namespace mpl {

template <typename IC, class Enable = typename enable_if< is_integral_constant<IC> >::type>
struct abs : mpl::integral_c<typename make_unsigned<typename IC::value_type>::type,
	(IC::value < 0 ? -IC::value : IC::value)
>
{};

}

template <typename T, T data, class Enable = typename enable_if< is_integral<T> >::type >
struct static_abs : mpl::abs< mpl::integral_c<T, data> >
{};

} // boost

#endif