/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_NEXT_IMPL_HPP
#define BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_NEXT_IMPL_HPP

#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace fusion
{
    struct joint_view_iterator_tag;

    template <typename First, typename Last, typename Concat>
    struct joint_view_iterator;

    namespace extension
    {
        template <typename Tag>
        struct next_impl;

        template <>
        struct next_impl<joint_view_iterator_tag>
        {
            template <typename ItRef>
            struct apply
            {
                typedef typename detail::remove_reference<ItRef>::type it;
                typedef typename it::first_type first_type;
                typedef typename it::last_type last_type;
                typedef typename it::concat_type concat_type;
                typedef typename result_of::next<first_type>::type next_type;
                typedef result_of::equal_to<next_type, last_type> equal_to;

                typedef typename
                    mpl::if_<
                        equal_to
                      , concat_type
                      , joint_view_iterator<next_type, last_type, concat_type>
                    >::type
                type;

                static type
                call(ItRef it, mpl::true_)
                {
                    return it.concat;
                }

                static type
                call(ItRef it, mpl::false_)
                {
                    return type(fusion::next(it.first), it.concat);
                }

                static type
                call(ItRef it)
                {
                    return call(it, equal_to());
                }
            };
        };
    }
}}

#endif
