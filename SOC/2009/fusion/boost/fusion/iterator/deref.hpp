/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ITERATOR_DEREF_HPP
#define BOOST_FUSION_ITERATOR_DEREF_HPP

#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/internal/assert.hpp>

namespace boost { namespace fusion
{
    struct iterator_facade_tag;

    namespace extension
    {
        template <typename>
        struct deref_impl;

        template <>
        struct deref_impl<iterator_facade_tag>
        {
            template <typename It>
            struct apply
              : detail::remove_reference<It>::type::template deref<It>
            {};
       };
    }

    namespace result_of
    {
        template <typename It>
        struct deref
          : extension::deref_impl<typename traits::tag_of<It>::type>::
                template apply<It>
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_iterator<It>));
        };
    }

    template <typename It>
    typename result_of::deref<It const&>::type
    deref(It const& it)
    {
        return result_of::deref<It const&>::call(it);
    }

    template <typename It>
    typename result_of::deref<It const&>::type
    operator*(iterator_base<It> const& it)
    {
        return fusion::deref(it.cast());
    }
}}

#endif
