/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_BEGIN_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_BEGIN_HPP

#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/internal/assert.hpp>

namespace boost { namespace fusion
{
    struct sequence_facade_tag;

    namespace extension
    {
        template <typename>
        struct begin_impl;

        template <>
        struct begin_impl<sequence_facade_tag>
        {
            template <typename Seq>
            struct apply
              : detail::remove_reference<Seq>::type::template begin<Seq>
            {};
        };
    }

    namespace result_of
    {
        template <typename Seq>
        struct begin
            : extension::begin_impl<typename traits::tag_of<Seq>::type>::
                template apply<Seq>
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));
        };
    }

    template <typename Seq>
    inline typename
        result_of::begin<BOOST_FUSION_R_ELSE_CLREF(Seq)>::type
    begin(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return result_of::begin<BOOST_FUSION_R_ELSE_CLREF(Seq)>::call(seq);
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Seq>
    inline typename result_of::begin<Seq&>::type
    begin(Seq& seq)
    {
        return result_of::begin<Seq&>::call(seq);
    }
#endif

}}

#endif
