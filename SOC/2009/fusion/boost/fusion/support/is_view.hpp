/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_IS_VIEW_HPP
#define BOOST_FUSION_SUPPORT_IS_VIEW_HPP

#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

namespace boost { namespace fusion
{
    namespace extension
    {
        template<typename>
        struct is_view_impl
        {
            template <typename Seq>
            struct apply
              :  detail::remove_reference<Seq>::type::is_view
            {};
        };
    }

    namespace traits
    {
        template <typename Seq>
        struct is_view
          : extension::is_view_impl<typename fusion::traits::tag_of<Seq>::type>::
                template apply<Seq>::type
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
        };
    }
}}

#endif
