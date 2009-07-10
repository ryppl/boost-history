/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_DETAIL_EQUAL_TO_IMPL_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_DETAIL_EQUAL_TO_IMPL_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template <typename It1, typename It2>
        struct vector_types_equal
          : is_same<
                typename detail::identity<typename It1::vector>::type
              , typename detail::identity<typename It2::vector>::type
            >
        {};
    }

    namespace extension
    {
        template <typename Tag>
        struct equal_to_impl;

        template <>
        struct equal_to_impl<vector_iterator_tag>
        {
            template <typename It1Ref, typename It2Ref>
            struct apply
            {
                typedef typename detail::remove_reference<It1Ref>::type it1;
                typedef typename detail::remove_reference<It2Ref>::type it2;

                typedef
                    mpl::and_<
                        mpl::equal_to<typename it1::index, typename it2::index>
                      , detail::vector_types_equal<it1,it2>
                    >
                type;
            };
        };
    }
}}

#endif
