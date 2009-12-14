/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_DETAIL_VARIADIC_TEMPLATES_TAGGED_IMPL_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_DETAIL_VARIADIC_TEMPLATES_TAGGED_IMPL_HPP

#include <boost/mpl/int.hpp>
#include <boost/type_traits/add_const.hpp>

namespace boost { namespace fusion { namespace detail
{
    template<int I, typename Element>
    struct element;

    template<int I, typename T>
    static T
    at_type_helper(element<I,T>const&);

    template<int I, typename T>
    static typename add_lref<T>::type
    at_helper(element<I,T>& element)
    {
        return element._element;
    }

    template<int I, typename T>
    static typename add_lref<typename add_const<T>::type>::type
    at_helper(element<I,T>const& element)
    {
        return element._element;
    }
}}}

#endif
