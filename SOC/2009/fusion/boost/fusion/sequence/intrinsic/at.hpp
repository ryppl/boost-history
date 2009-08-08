/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_AT_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_AT_HPP

#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/ref.hpp>

#include <boost/mpl/int.hpp>

#if defined(BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS) || BOOST_WORKAROUND(__GNUC__,<4)
#   include <boost/type_traits/is_const.hpp>
#   include <boost/utility/enable_if.hpp>
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
        {};

        template <typename Seq, int N>
        struct at_c
          : at<Seq, mpl::int_<N> >
        {};
    }

    template <typename N, typename Seq>
    inline typename result_of::at<BOOST_FUSION_R_ELSE_CLREF(Seq), N>::type
    at(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return result_of::at<BOOST_FUSION_R_ELSE_CLREF(Seq), N>::call(seq);
    }

    //cschmidt: see https://svn.boost.org/trac/boost/ticket/3305
#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename N, typename Seq>
#if defined(BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS) || BOOST_WORKAROUND(__GNUC__,<4)
    inline typename lazy_disable_if<is_const<Seq>,result_of::at<Seq&, N> >::type
#else
    inline typename result_of::at<Seq&, N>::type
#endif
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
#if defined(BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS) || BOOST_WORKAROUND(__GNUC__,<4)
    inline typename
        lazy_disable_if<is_const<Seq>,result_of::at_c<Seq&, N> >::type
#else
    inline typename result_of::at_c<Seq&, N>::type
#endif
    at_c(Seq& seq)
    {
        return fusion::at<mpl::int_<N> >(seq);
    }
#endif
}}

#endif
