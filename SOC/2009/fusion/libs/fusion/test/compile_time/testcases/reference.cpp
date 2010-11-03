/*==============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifdef BOOST_FUSION_USE_TYPE_TRAITS
#   include <boost/type_traits/add_reference.hpp>
#   include <boost/type_traits/remove_reference.hpp>
#   include <boost/type_traits/is_reference.hpp>

#   define BOOST_FUSION_ADD_REFERENCE boost::add_reference
#   define BOOST_FUSION_REMOVE_REFERENCE boost::remove_reference
#   define BOOST_FUSION_IS_REFERENCE boost::is_reference
#else
#   include <boost/config.hpp>
#   include <boost/fusion/support/internal/base/config.hpp>
#   include <boost/fusion/support/internal/base/ref_impl.hpp>

#   define BOOST_FUSION_ADD_REFERENCE boost::fusion::detail::add_lref
#   define BOOST_FUSION_REMOVE_REFERENCE boost::fusion::detail::remove_reference
#   define BOOST_FUSION_IS_REFERENCE boost::fusion::detail::is_lrref
#endif

int result=0;

template<int>
struct bar
{};

template<typename>
struct foo
{
    static const int i=0;
};

template<int I>
struct foo<bar<I> const>
{
    static const int i=1;
};

template<int I>
void
test_impl()
{
    result+=foo<typename BOOST_FUSION_ADD_REFERENCE<bar<I> >::type>::i;
    result+=foo<typename BOOST_FUSION_ADD_REFERENCE<bar<I> const>::type>::i;
    result+=foo<typename BOOST_FUSION_ADD_REFERENCE<bar<I>&>::type>::i;
    result+=foo<typename BOOST_FUSION_ADD_REFERENCE<bar<I> const&>::type>::i;
#ifndef BOOST_NO_RVALUE_REFERENCES
    result+=foo<typename BOOST_FUSION_ADD_REFERENCE<bar<I>&&>::type>::i;
    result+=foo<
        typename BOOST_FUSION_ADD_REFERENCE<bar<I> const&&>::type
    >::i;
#endif

    result+=foo<typename BOOST_FUSION_REMOVE_REFERENCE<bar<I> >::type>::i;
    result+=foo<typename BOOST_FUSION_REMOVE_REFERENCE<bar<I> const>::type>::i;
    result+=foo<typename BOOST_FUSION_REMOVE_REFERENCE<bar<I>&>::type>::i;
    result+=foo<typename BOOST_FUSION_REMOVE_REFERENCE<bar<I> const&>::type>::i;
#ifndef BOOST_NO_RVALUE_REFERENCES
    result+=foo<typename BOOST_FUSION_REMOVE_REFERENCE<bar<I>&&>::type>::i;
    result+=foo<
        typename BOOST_FUSION_REMOVE_REFERENCE<bar<I> const&&>::type
    >::i;
#endif

    result+=BOOST_FUSION_IS_REFERENCE<bar<I> >::type::value ? 1 : 0;
    result+=BOOST_FUSION_IS_REFERENCE<bar<I> const>::type::value ? 1 : 0;
    result+=BOOST_FUSION_IS_REFERENCE<bar<I>&>::type::value ? 1 : 0;
    result+=BOOST_FUSION_IS_REFERENCE<bar<I> const&>::type::value ? 1 : 0;
#ifndef BOOST_NO_RVALUE_REFERENCES
    result+=BOOST_FUSION_IS_REFERENCE<bar<I>&&>::type::value ? 1 : 0;
    result+=BOOST_FUSION_IS_REFERENCE<bar<I> const&&>::value ? 1 : 0;
#endif
}

#include "detail/main.hpp"
