//  Boost integer/is_integral_constant.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  (C) Copyright Vicente J. Botet Escribá 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STATIC_IS_INTEGRAL_CONSTANT_INCLUDED
#define BOOST_STATIC_IS_INTEGRAL_CONSTANT_INCLUDED

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost {

/*
 *	is_integral_constant<T> checks if T is or not an integral_constant
 */

namespace mpl {

//~ template <typename IC>
//~ struct is_integral_constant : false_ {};

    
//~ namespace detail {
BOOST_MPL_HAS_XXX_TRAIT_DEF(tag)
BOOST_MPL_HAS_XXX_TRAIT_DEF(value_type)
//~ BOOST_MPL_HAS_XXX_TRAIT_DEF(value)
BOOST_MPL_HAS_XXX_TRAIT_DEF(type)
//~ }
template <typename IC>
struct is_integral_constant : and_<
		and_< has_tag<IC>, is_same<typename IC::tag, integral_c_tag> >,
		and_< has_value_type<IC>, is_integral<typename IC::value_type> >,
		//detail_has_value<IC>,
		has_type<IC>
	>
{};

    
    
template <typename T, T value>
struct is_integral_constant<integral_c<T,value> > : true_ {};
    
}


} // boost

#endif
