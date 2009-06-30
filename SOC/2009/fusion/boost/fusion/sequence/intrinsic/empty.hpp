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
            template <typename Sequence>
            struct apply
                : mpl::bool_<(result_of::size<Sequence>::value == 0)>
            {};
        };

        template <>
        struct empty_impl<sequence_facade_tag>
        {
            template <typename Sequence>
            struct apply :
                detail::remove_reference<Sequence>::type::template empty<Sequence>
            {};
        };
    }

    namespace result_of
    {
        template <typename Sequence>
        struct empty
            : extension::empty_impl<typename traits::tag_of<Sequence>::type>::
                template apply<typename detail::add_lref<Sequence>::type>
        {};
    }

    template <typename Sequence>
    inline typename result_of::empty<Sequence const&>::type
    empty(Sequence const&)
    {
        typedef typename result_of::empty<Sequence const&>::type result;
        return result();
    }
}}

#endif
