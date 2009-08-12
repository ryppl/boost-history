/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_AT_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_AT_HPP

#ifdef BOOST_FUSION_ENABLE_STATIC_ASSERTS
#   include <boost/fusion/container/list/list_fwd.hpp>
#   include <boost/fusion/sequence/intrinsic/size.hpp>
#endif
#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/detail/workaround.hpp>
#include <boost/fusion/support/assert.hpp>

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
        struct at_impl;

        template <>
        struct at_impl<sequence_facade_tag>
        {
            template <typename SeqRef, typename N>
            struct apply
              : detail::remove_reference<SeqRef>::type::template at<SeqRef, N>
            {};
        };
    }

    namespace result_of
    {
        template <typename Seq, typename N>
        struct at
          : extension::at_impl<typename traits::tag_of<Seq>::type>::
                template apply<typename detail::add_lref<Seq>::type, N>
        {
            //BOOST_FUSION_MPL_ASSERT((traits_is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((
                mpl::or_<
                    traits::is_random_access<Seq>
                  , is_same<
                        typename traits::tag_of<Seq>::type
                      , list_tag
                    >
                >));
            BOOST_FUSION_INDEX_CHECK(N::value,size<Seq>::value);
        };

        template <typename Seq, int N>
        struct at_c
          : at<Seq, mpl::int_<N> >
        {
            //BOOST_FUSION_MPL_ASSERT((traits_is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((
                mpl::or_<
                    traits::is_random_access<Seq>
                  , is_same<
                        typename traits::tag_of<Seq>::type
                      , list_tag
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
