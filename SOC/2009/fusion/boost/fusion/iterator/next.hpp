/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ITERATOR_NEXT_HPP
#define BOOST_FUSION_ITERATOR_NEXT_HPP

#include <boost/fusion/support/tag_of.hpp>

namespace boost { namespace fusion
{
    // Special tags:
    struct iterator_facade_tag; // iterator facade tag

    namespace extension
    {
        template <typename Tag>
        struct next_impl;

        template <>
        struct next_impl<iterator_facade_tag>
        {
            template <typename ItRef>
            struct apply
              : detail::remove_reference<ItRef>::type::template next<ItRef>
            {};
        };
    }

    namespace result_of
    {
        template <typename It>
        struct next
          : extension::next_impl<typename traits::tag_of<It>::type>::
                template apply<typename detail::add_lref<It>::type>
        {};
    }

    template <typename It>
    typename result_of::next<It const&>::type const
    next(It const& it)
    {
        return result_of::next<It const&>::call(it);
    }
}}

#endif
