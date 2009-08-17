/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_IS_VIEW_HPP
#define BOOST_FUSION_SUPPORT_IS_VIEW_HPP

#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/internal/ref.hpp>

namespace boost { namespace fusion
{
    namespace extension
    {
        template<typename Tag>
        struct is_view_impl
        {
            template <typename SeqRef>
            struct apply
              :  detail::remove_reference<SeqRef>::type::is_view
            {};
        };
    }

    namespace traits
    {
        template <typename T>
        struct is_view
          : extension::is_view_impl<typename fusion::traits::tag_of<T>::type>::
                template apply<typename detail::add_lref<T>::type>::type
        {};
    }
}}

#endif
