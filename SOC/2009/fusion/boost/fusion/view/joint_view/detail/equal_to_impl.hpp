/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_EQUAL_TO_IMPL_HPP
#define BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_EQUAL_TO_IMPL_HPP

#include <boost/fusion/iterator/equal_to.hpp>

#include <boost/mpl/and.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template<typename It1, typename It2>
        struct same_joint_view_iterator
          : result_of::equal_to<
                typename It1::begin_type
              , typename It2::begin_type
            >
        {};
    }

    namespace extension
    {
        template<typename>
        struct equal_to_impl;

        template<>
        struct equal_to_impl<joint_view_iterator_tag>
        {
            template<typename It1, typename It2>
            struct apply
            {
                typedef typename detail::remove_reference<It1>::type it1;
                typedef typename detail::remove_reference<It2>::type it2;

                typedef
                    mpl::and_<
                        is_same<
                            typename it1::fusion_tag
                          , typename it2::fusion_tag
                        >
                      , detail::same_joint_view_iterator<it1,it2>
                    >
                type;
            };
        };

        template <>
        struct equal_to_impl<concat_iterator_tag>
          : equal_to_impl<joint_view_iterator_tag>
        {};
    }
}}

#endif
