/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_EMPTY_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_EMPTY_HPP

#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/support/tag_of.hpp>

#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion
{
    // Special tags:
    struct sequence_facade_tag;

    namespace extension
    {
        template <typename Tag>
        struct empty_impl
        {
            template <typename SeqRef>
            struct apply
              : mpl::bool_<(result_of::size<SeqRef>::value == 0)>
            {};
        };

        template <>
        struct empty_impl<sequence_facade_tag>
        {
            template <typename SeqRef>
            struct apply
              : detail::remove_reference<SeqRef>::type::template empty<SeqRef>
            {};
        };
    }

    namespace result_of
    {
        template <typename Seq>
        struct empty
            : extension::empty_impl<typename traits::tag_of<Seq>::type>::
                template apply<typename detail::add_lref<Seq>::type>
        {};
    }

    template <typename Seq>
    inline typename result_of::empty<Seq const&>::type
    empty(Seq const&)
    {
        return typename result_of::empty<Seq const&>::type();
    }
}}

#endif
