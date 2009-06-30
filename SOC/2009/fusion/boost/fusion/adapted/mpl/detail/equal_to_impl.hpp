/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_MPL_DETAIL_EQUAL_TO_IMPL_HPP
#define BOOST_FUSION_ADAPTED_MPL_DETAIL_EQUAL_TO_IMPL_HPP

#include <boost/fusion/adapted/mpl/detail/get_mpl_it.hpp>

#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    struct mpl_iterator_tag;

    namespace extension
    {
        template <typename Tag>
        struct equal_to_impl;

        template <>
        struct equal_to_impl<mpl_iterator_tag>
        {
            template <typename I1Ref, typename I2Ref>
            struct apply
              : is_same<
                    typename detail::identity<I1Ref>::type
                  , typename detail::get_mpl_it<
                        typename detail::identity<I2Ref>::type
                    >::type
                >
            {
            };
        };
    }
}}

#endif
