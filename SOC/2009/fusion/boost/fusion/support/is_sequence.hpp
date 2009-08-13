/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_IS_SEQUENCE_HPP
#define BOOST_FUSION_SUPPORT_IS_SEQUENCE_HPP

#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/sequence_base.hpp>

#include <boost/type_traits/is_base_of.hpp>

namespace boost { namespace fusion
{
    namespace extension
    {
        template <typename Tag>
        struct is_sequence_impl
        {
            template <typename SeqRef>
            struct apply
              : is_base_of<
                    sequence_root
                  , typename detail::identity<SeqRef>::type
                >
            {};
        };
    }

    namespace traits
    {
        template <typename T>
        struct is_sequence
          : extension::is_sequence_impl<typename traits::tag_of<T>::type>::
                template apply<typename detail::add_lref<T>::type>
        {};
    }
}}

#endif
