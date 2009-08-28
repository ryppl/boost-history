/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ITERATOR_VALUE_OF_HPP
#define BOOST_FUSION_ITERATOR_VALUE_OF_HPP

#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/internal/assert.hpp>

namespace boost { namespace fusion
{
    struct iterator_facade_tag;

    namespace extension
    {
        template <typename>
        struct value_of_impl;

        template <>
        struct value_of_impl<iterator_facade_tag>
        {
            template <typename It>
            struct apply
              : detail::remove_reference<It>::type::template value_of<It>
            {};
        };
    }

    namespace result_of
    {
        template <typename It>
        struct value_of
          : extension::value_of_impl<typename traits::tag_of<It>::type>::
                template apply<It>
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_iterator<It>));
        };
    }
}}

#endif
