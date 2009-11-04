/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_NEXT_IMPL_HPP
#define BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_NEXT_IMPL_HPP

#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct next_impl;

    template <>
    struct next_impl<joint_view_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;

            typedef typename it::begin_type begin_type;
            typedef typename it::end_type end_type;
            typedef typename it::concat_type concat_type;
            typedef typename result_of::next<begin_type>::type next_type;
            typedef typename
                result_of::equal_to<next_type, end_type>::type
            equal_to;

            typedef typename
                mpl::if_<
                    equal_to
                  , concat_iterator<typename it::category, concat_type>
                  , joint_view_iterator<
                        typename it::category
                      , next_type
                      , end_type
                      , concat_type
                    >
                >::type
            type;

            static type
            call(It it, mpl::true_)
            {
                return type(it.concat,0);
            }

            static type
            call(It it, mpl::false_)
            {
                return type(fusion::next(it.first), it.concat);
            }

            static type
            call(It it)
            {
                return call(it, equal_to());
            }
        };
    };

    template <>
    struct next_impl<concat_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;

            typedef
                concat_iterator<
                    typename it::category
                  , typename result_of::next<typename it::begin_type>::type
                >
            type;

            static type
            call(It it)
            {
                return type(fusion::next(it.first),0);
            }
        };
    };
}}}

#endif
