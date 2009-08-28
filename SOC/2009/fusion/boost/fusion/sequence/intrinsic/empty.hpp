/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_EMPTY_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_EMPTY_HPP

#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/fusion/support/internal/ref.hpp>

#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion
{
    struct sequence_facade_tag;

    namespace extension
    {
        template <typename>
        struct empty_impl
        {
            template <typename Seq>
            struct apply
              : mpl::bool_<!result_of::size<Seq>::value>
            {};
        };

        template <>
        struct empty_impl<sequence_facade_tag>
        {
            template <typename Seq>
            struct apply
              : detail::remove_reference<Seq>::type::template empty<Seq>
            {};
        };
    }

    namespace result_of
    {
        template <typename Seq>
        struct empty
          : extension::empty_impl<typename traits::tag_of<Seq>::type>::
                template apply<Seq>::type
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));
        };
    }

    template <typename Seq>
    inline typename result_of::empty<Seq const&>::type
    empty(Seq const&)
    {
        return typename result_of::empty<Seq const&>::type();
    }
}}

#endif
