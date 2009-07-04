/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_BEGIN_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_BEGIN_HPP

#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/tag_of.hpp>

namespace boost { namespace fusion
{
    // Special tags:
    struct sequence_facade_tag; // iterator facade tag

    namespace extension
    {
        template <typename Tag>
        struct begin_impl;

        template <>
        struct begin_impl<sequence_facade_tag>
        {
            template <typename Sequence>
            struct apply :
                detail::remove_reference<Sequence>::type::template begin<Sequence>
            {};
        };
    }

    namespace result_of
    {
        template <typename Sequence>
        struct begin
            : extension::begin_impl<typename traits::tag_of<Sequence>::type>::
                template apply<typename detail::add_lref<Sequence>::type>
        {};
    }

    //TODO cschmidt: const retval?!

    template <typename Sequence>
    inline typename
        result_of::begin<BOOST_FUSION_R_ELSE_CLREF(Sequence)>::type
    begin(BOOST_FUSION_R_ELSE_CLREF(Sequence) seq)
    {
        return result_of::begin<BOOST_FUSION_R_ELSE_CLREF(Sequence)>::call(
                BOOST_FUSION_FORWARD(Sequence,seq));
    }

}}

#endif
