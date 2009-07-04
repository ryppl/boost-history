/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_END_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_END_HPP

#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/tag_of.hpp>

namespace boost { namespace fusion
{
    // Special tags:
    struct sequence_facade_tag;

    namespace extension
    {
        template <typename Tag>
        struct end_impl;

        template <>
        struct end_impl<sequence_facade_tag>
        {
            template <typename Sequence>
            struct apply :
                detail::remove_reference<Sequence>::type::template end<Sequence>
            {};
        };
    }

    namespace result_of
    {
        template <typename Sequence>
        struct end
            : extension::end_impl<typename traits::tag_of<Sequence>::type>::
                template apply<typename detail::add_lref<Sequence>::type>
        {};
    }

    template <typename Sequence>
    inline typename
        result_of::end<BOOST_FUSION_R_ELSE_CLREF(Sequence)>::type const
    end(BOOST_FUSION_R_ELSE_CLREF(Sequence) seq)
    {
        return result_of::end<BOOST_FUSION_R_ELSE_CLREF(Sequence)>::call(
                BOOST_FUSION_FORWARD(Sequence,seq));
    }

}}

#endif
