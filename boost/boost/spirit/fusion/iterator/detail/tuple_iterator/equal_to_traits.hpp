/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_ITERATOR_DETAIL_TUPLE_ITERATOR_EQUAL_TO_TRAITS_HPP)
#define FUSION_ITERATOR_DETAIL_TUPLE_ITERATOR_EQUAL_TO_TRAITS_HPP

#include <boost/spirit/fusion/detail/config.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/and.hpp>

namespace boost { namespace fusion
{
    struct tuple_iterator_tag;

    template <typename Tag>
    struct equal_to_traits;

    namespace detail
    {
        template <typename I1, typename I2>
        struct has_same_tags
            : is_same<FUSION_GET_TAG(I1), FUSION_GET_TAG(I2)> {};

        template <typename I1, typename I2>
        struct has_same_index
            : mpl::equal_to<FUSION_GET_INDEX(I1), FUSION_GET_INDEX(I2)>::type {};

        template <typename I1, typename I2>
        struct has_same_tuple_type
            : is_same<
                typename add_const<FUSION_GET_TUPLE(I1)>::type
              , typename add_const<FUSION_GET_TUPLE(I2)>::type
            >
        {};

        template <typename I1, typename I2>
        struct tuple_iterator_equal_to
            : mpl::and_<
                has_same_index<I1, I2>
              , has_same_tuple_type<I1, I2>
            >
        {
            BOOST_STATIC_ASSERT((has_same_tags<I1, I2>::value));
        };
    }

    template <>
    struct equal_to_traits<tuple_iterator_tag>
    {
        template <typename I1, typename I2>
        struct impl : detail::tuple_iterator_equal_to<I1, I2> {};
    };
}}

#endif

