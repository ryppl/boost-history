/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ITERATOR_ADVANCE_HPP
#define BOOST_FUSION_ITERATOR_ADVANCE_HPP

#include <boost/fusion/iterator/detail/advance.hpp>
#include <boost/fusion/support/assert.hpp>
#include <boost/fusion/support/tag_of.hpp>

namespace boost { namespace fusion
{
    struct random_access_traversal_tag;
    struct iterator_facade_tag;

    namespace extension
    {
        template <typename Tag>
        struct advance_impl
        {
            // default implementation
            template <typename ItRef, typename N>
            struct apply
              : mpl::if_c<
                    (N::value > 0)
                  , advance_detail::forward<ItRef, N::value>
                  , advance_detail::backward<ItRef, N::value>
                >::type
            {
//                BOOST_FUSION_MPL_ASSERT_NOT(
//                        traits::is_random_access<ItRef>,
//            "default implementation not available for random access iterators");
            };
        };

        template <>
        struct advance_impl<iterator_facade_tag>
        {
            template <typename ItRef, typename N>
            struct apply
              : detail::remove_reference<ItRef>::type::
                    template advance<ItRef, N>
            {};
        };
    }

    namespace result_of
    {
        template <typename It, int N>
        struct advance_c
          : extension::advance_impl<typename traits::tag_of<It>::type>::
                template apply<
                    typename detail::add_lref<It>::type
                  , mpl::int_<N>
                >
        {};

        template <typename It, typename N>
        struct advance
          : extension::advance_impl<typename traits::tag_of<It>::type>::
                template apply<typename detail::add_lref<It>::type, N>
        {};
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
