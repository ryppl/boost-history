/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ITERATOR_ADVANCE_HPP
#define BOOST_FUSION_ITERATOR_ADVANCE_HPP

#include <boost/fusion/iterator/detail/advance.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/fusion/support/tag_of.hpp>

namespace boost { namespace fusion
{
    struct random_access_traversal_tag;
    struct iterator_facade_tag;

    namespace extension
    {
        template <typename>
        struct advance_impl
        {
            template <typename It, typename N>
            struct apply
              : mpl::if_c<
                    (N::value > 0)
                  , detail::forward<It, N::value>
                  , detail::backward<It, N::value>
                >::type
            {};
        };

        template <>
        struct advance_impl<iterator_facade_tag>
        {
            template <typename It, typename N>
            struct apply
              : detail::remove_reference<It>::type::template advance<It, N>
            {};
        };
    }

    namespace result_of
    {
        template <typename It, int N>
        struct advance_c
          : extension::advance_impl<typename traits::tag_of<It>::type>::
                template apply<It, mpl::int_<N> >
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_iterator<It>));
            BOOST_FUSION_STATIC_ASSERT((
                    N>=0 ||
                    traits::is_bidirectional<It>::value));

        };

        template <typename It, typename N>
        struct advance
          : extension::advance_impl<typename traits::tag_of<It>::type>::
                template apply<It, N>
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_iterator<It>));
            BOOST_FUSION_STATIC_ASSERT((
                    N::value>=0 ||
                    traits::is_bidirectional<It>::value));
        };
    }

    template <int N, typename It>
    inline typename result_of::advance_c<It const&, N>::type const
    advance_c(It const& it)
    {
        return result_of::advance_c<It const&, N>::call(it);
    }

    template<typename N, typename It>
    inline typename result_of::advance<It const&, N>::type const
    advance(It const& it)
    {
        return result_of::advance<It const&, N>::call(it);
    }
}}

#endif
