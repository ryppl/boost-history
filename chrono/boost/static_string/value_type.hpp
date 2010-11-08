//  value_type.hpp  --------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef BOOST_STATIC_STRING_VALUE_TYPE_HPP
#define BOOST_STATIC_STRING_VALUE_TYPE_HPP

#include <boost/mpl/front.hpp>
#include <boost/mpl/has_xxx.hpp>

namespace boost {
namespace static_string {

template< typename X > 
struct value_type;

namespace detail {

BOOST_MPL_HAS_XXX_TRAIT_DEF( value_type )

template< typename X, bool IsSeq=static_string::detail::has_value_type<X>::value > 
struct value_type_impl_is_sequence;


template< typename Seq> 
struct value_type_impl_is_sequence<Seq, true>
{
    typedef typename value_type<typename mpl::front<Seq>::type >::type type;
};

//template< typename X> 
//struct value_type_impl_is_sequence<X, false>,
//{
//    typedef typename value_type< typename mpl::front<X>::type >::type type;
//};

template< typename X, bool HasValueType=static_string::detail::has_value_type<X>::value >
struct value_type_impl_has_value_type;

template< typename X>
struct value_type_impl_has_value_type<X,true>
{
    typedef typename X::value_type type;
};

template< typename X > 
struct value_type_impl_has_value_type<X,false>
{
    typedef typename value_type< typename mpl::front<X>::type >::type type;
};

}

template< typename X > 
struct value_type
    : static_string::detail::value_type_impl_has_value_type< X >
{
};


} // namespace static_string
} // namespace boost

#endif // BOOST_STATIC_STRING_VALUE_TYPE_HPP
