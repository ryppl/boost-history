/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_AT_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_AT_HPP

#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/workaround.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/mpl/int.hpp>

namespace boost { namespace fusion
{
    struct sequence_facade_tag;
#ifdef BOOST_FUSION_ENABLE_STATIC_ASSERTS
    struct list_tag;
    struct cons_tag;
#endif

    namespace extension
    {
        template <typename>
        struct at_impl;

        template <>
        struct at_impl<sequence_facade_tag>
        {
            template <typename Seq, typename N>
            struct apply
              : detail::remove_reference<Seq>::type::template at<Seq, N>
            {};
        };
    }

    namespace result_of
    {
        template <typename Seq, typename N>
        struct at
          : extension::at_impl<typename traits::tag_of<Seq>::type>::
                template apply<Seq, N>
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((
                mpl::or_<
                    traits::is_random_access<Seq>
                  , is_same<
                        typename traits::tag_of<Seq>::type
                      , list_tag
                    >
                  , is_same<
                        typename traits::tag_of<Seq>::type
                      , cons_tag
                    >
                >));

            BOOST_FUSION_INDEX_CHECK(N::value,size<Seq>::value);
        };

        template <typename Seq, int N>
        struct at_c
          : at<Seq, mpl::int_<N> >
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((
                mpl::or_<
                    traits::is_random_access<Seq>
                  , is_same<
                        typename traits::tag_of<Seq>::type
                      , list_tag
                    >
                  , is_same<
                        typename traits::tag_of<Seq>::type
                      , cons_tag
                    >
                >));
            BOOST_FUSION_INDEX_CHECK(N,size<Seq>::value);
        };
    }

    template <typename N, typename Seq>
    inline typename result_of::at<BOOST_FUSION_R_ELSE_CLREF(Seq), N>::type
    at(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return result_of::at<BOOST_FUSION_R_ELSE_CLREF(Seq), N>::call(seq);
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename N, typename Seq>
    inline BOOST_FUSION_EXPLICIT_TEMPLATE_NON_CONST_ARG_OVERLOAD(
            result_of::at<,Seq,&,N>)
    at(Seq& seq)
    {
        return result_of::at<Seq&, N>::call(seq);
    }
#endif

    template <int N, typename Seq>
    inline typename
        result_of::at_c<BOOST_FUSION_R_ELSE_CLREF(Seq), N>::type
    at_c(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return fusion::at<mpl::int_<N> >(seq);
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <int N, typename Seq>
    inline BOOST_FUSION_EXPLICIT_TEMPLATE_NON_CONST_ARG_OVERLOAD(
            result_of::at_c<,Seq,&, N>)
    at_c(Seq& seq)
    {
        return fusion::at<mpl::int_<N> >(seq);
    }
#endif
}}

#endif
