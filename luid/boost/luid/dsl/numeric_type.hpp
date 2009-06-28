/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_DSL_NUMERIC_TYPE_HPP_
#define BOOST_LUID_DSL_NUMERIC_TYPE_HPP_

#include "boost/integer_traits.hpp"
#include <boost/detail/is_xxx.hpp>
#include <boost/parameter/aux_/void.hpp>
#include <boost/parameter/parameters.hpp>
#include <boost/type_traits.hpp>


namespace boost {
namespace luid {
namespace dsl {

#if 0
template<typename T=parameter::void_>
struct numeric_type: dsl::terminal<numeric_type<>,T>
{};

#endif


struct numeric_type_marker{};

template<typename T>
struct is_numeric_type: mpl::or_<is_base_and_derived<numeric_type_marker,T>, 
                                boost::is_unsigned<T> >
{
};

template<typename T=parameter::void_>
struct numeric_type: parameter::template_keyword<numeric_type<>,T>
{};


template <typename T=unsigned int, T lower_bound_=boost::integer_traits<T>::const_min> 
struct static_lower_bound : numeric_type_marker {
    typedef T numeric_type;
    static const T lower_bound = lower_bound_;
};

template <typename T> 
struct is_static_lower_bound: mpl::false_ {};

template <typename T, T lower_bound_> 
struct is_static_lower_bound<static_lower_bound<T, lower_bound_> >: mpl::true_ {};


template <typename T=unsigned int, T lower_bound_=boost::integer_traits<T>::const_min> 
struct dynamic_lower_bound : numeric_type_marker{
    typedef T numeric_type;
    static const T lower_bound = lower_bound_;
};

template <typename T> 
struct is_dynamic_lower_bound: mpl::false_ {};

template <typename T, T lower_bound_> 
struct is_dynamic_lower_bound<dynamic_lower_bound<T, lower_bound_> >: mpl::true_ {};



} // dsl
} // luid

template <typename T, T lower_bound_> 
struct integer_traits<luid::dsl::static_lower_bound<T, lower_bound_> > : integer_traits<T> {};

template <typename T, T lower_bound_> 
struct integer_traits<luid::dsl::dynamic_lower_bound<T, lower_bound_> > : integer_traits<T> {};

}// boost
#endif // BOOST_LUID_DSL_NUMERIC_TYPE_HPP_
