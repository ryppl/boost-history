/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_VALUE_AT_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_VALUE_AT_HPP

#ifdef BOOST_FUSION_ENABLE_STATIC_ASSERTS
#   include <boost/fusion/container/list/list_fwd.hpp>
#   include <boost/fusion/sequence/intrinsic/size.hpp>
#endif
#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <boost/mpl/int.hpp>
#ifdef BOOST_FUSION_ENABLE_STATIC_ASSERTS
#   include <boost/mpl/or.hpp>
#   include <boost/type_traits/is_same.hpp>
#endif

namespace boost { namespace fusion
{
    struct sequence_facade_tag;

    namespace extension
    {
        template <typename Tag>
        struct value_at_impl;

        template <>
        struct value_at_impl<sequence_facade_tag>
        {
            template <typename SeqRef, typename N>
            struct apply
              : detail::remove_reference<SeqRef>::type::
                    template value_at<SeqRef, N>
            {};
        };
    }

    namespace result_of
    {
        template <typename Seq, typename N>
        struct value_at
          : extension::value_at_impl<typename traits::tag_of<Seq>::type>::
                template apply<typename detail::add_lref<Seq>::type, N>
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((
                mpl::or_<
                    traits::is_random_access<Seq>
                  , is_same<
                        typename traits::tag_of<Seq>::type
                      , list_tag
                    >
                >));
            BOOST_FUSION_INDEX_CHECK(N::value,size<Seq>::value);
            BOOST_FUSION_INDEX_CHECK(N::value,size<Seq>::value);
        };

        template <typename Seq, int N>
        struct value_at_c
          : result_of::value_at<Seq,mpl::int_<N> >
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((
                mpl::or_<
                    traits::is_random_access<Seq>
                  , is_same<
                        typename traits::tag_of<Seq>::type
                      , list_tag
                    >
                >));
            BOOST_FUSION_INDEX_CHECK(N,size<Seq>::value);
            BOOST_FUSION_INDEX_CHECK(N,size<Seq>::value);
        };
    }
}}

#endif
