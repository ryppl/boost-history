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
#include <boost/type_traits/is_const.hpp>

namespace boost { namespace fusion
{
    // Special tags:
    struct sequence_facade_tag;

    namespace extension
    {
        template <typename Tag>
        struct at_impl
        {
            template <typename Sequence, typename N>
            struct apply;
        };

        template <>
        struct at_impl<sequence_facade_tag>
        {
            template <typename Sequence, typename N>
            struct apply
                : detail::remove_reference<Sequence>::type::template at<Sequence, N>
            {};
        };
    }

    namespace result_of
    {
        template <typename Sequence, typename N>
        struct at
            : extension::at_impl<typename traits::tag_of<Sequence>::type>::
                template apply<typename detail::add_lref<Sequence>::type, N>
        {};

        template <typename Sequence, int N>
        struct at_c
            : at<Sequence, mpl::int_<N> >
        {};
    }


    //TODO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    template <typename N, typename Sequence>
    inline typename result_of::at<BOOST_FUSION_R_ELSE_CLREF(Sequence), N>::type
    at(BOOST_FUSION_R_ELSE_CLREF(Sequence) seq)
    {
        return result_of::at<BOOST_FUSION_R_ELSE_CLREF(Sequence), N>::call(
                BOOST_FUSION_FORWARD(Sequence,seq));
    }

    template <int N, typename Sequence>
    inline typename
        result_of::at_c<BOOST_FUSION_R_ELSE_CLREF(Sequence), N>::type
    at_c(BOOST_FUSION_R_ELSE_CLREF(Sequence) seq)
    {
        return fusion::at<mpl::int_<N> >(BOOST_FUSION_FORWARD(Sequence,seq));
    }
}}

#endif
