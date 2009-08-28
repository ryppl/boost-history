/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_SIZE_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_SIZE_HPP

#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <boost/mpl/int.hpp>

namespace boost { namespace fusion
{
    struct sequence_facade_tag;

    namespace extension
    {
        template <typename>
        struct size_impl
        {
            template <typename Seq>
            struct apply
              : detail::remove_reference<Seq>::type::size
            {};
        };

        template <>
        struct size_impl<sequence_facade_tag>
        {
            template <typename Seq>
            struct apply
              : detail::remove_reference<Seq>::type::template size<Seq>::type
            {};
        };
    }

    namespace result_of
    {
        template <typename Seq>
        struct size
          : extension::size_impl<typename traits::tag_of<Seq>::type>::
                template apply<Seq>::type
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));
        };
    }

    template <typename Seq>
    inline typename result_of::size<Seq const&>::type
    size(Seq const&)
    {
        return typename result_of::size<Seq const&>::type();
    }
}}

#endif
