// (C) Copyright Christopher Diggins 2004.
// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Based on the class CBaseAspect from the code accompanying the 
// article "Aspect-Oriented Programming & C++", by Christopher Diggins, 
// Dr. Dobb's Journal, Aug 2004, p. 53.

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_ADVICE_CATEGORIES_HPP_INCLUDED
#define BOOST_IDL_ADVICE_CATEGORIES_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/interfaces/detail/empty_base.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace interfaces {

    // Advice category tags

struct null_advice_tag { 
    static const int id = 0;
};

struct basic_advice_tag : null_advice_tag { 
    static const int id = 1;
};

struct transforming_advice_tag : basic_advice_tag { 
    static const int id = 2;
};

template<typename T>
struct advice_category {
    typedef typename T::category type;
};

template<>
struct advice_category<detail::empty_base> {
    typedef null_advice_tag type;
};

namespace detail {

template<int Id>
struct advice_category_by_id;

template<>
struct advice_category_by_id<0> {
    typedef null_advice_tag type;
};

template<>
struct advice_category_by_id<1> {
    typedef basic_advice_tag type;
};

template<>
struct advice_category_by_id<2> {
    typedef transforming_advice_tag type;
};

} // End namespace detail.

template<typename Cat1, typename Cat2>
struct combine_categories {
    static const int max = 
        Cat1::id < Cat2::id ? Cat2::id : Cat1::id;
    typedef typename detail::advice_category_by_id<max>::type type;         
};

namespace detail {

template<typename T>
struct is_basic_impl { // Can't get mpl::not to work here on VC7.1.
    BOOST_STATIC_CONSTANT(
        bool, value = (!is_same<typename T::category, basic_advice_tag>::value)
    );
};

} // End namespace detail.

template<typename T>
struct is_basic_advice
    : mpl::bool_< detail::is_basic_impl<T>::value >
    { };

template<typename T>
struct is_transforming_advice 
    : is_same<typename T::category, transforming_advice_tag>
    { };

} } // End namespace interfaces, boost.

#endif // #ifndef BOOST_IDL_ADVICE_CATEGORIES_HPP_INCLUDED
