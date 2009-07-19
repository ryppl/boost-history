/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_VALUE_AT_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_VALUE_AT_HPP

#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/ref.hpp>

#include <boost/mpl/int.hpp>

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
        {};

        template <typename Seq, int N>
        struct value_at_c
          : fusion::result_of::value_at<Seq,mpl::int_<N> >
        {};
    }
}}

#endif
