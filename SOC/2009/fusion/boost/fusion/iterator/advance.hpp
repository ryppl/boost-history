/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ITERATOR_ADVANCE_HPP
#define BOOST_FUSION_ITERATOR_ADVANCE_HPP

#include <boost/fusion/iterator/detail/advance.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/assert.hpp>

#include <boost/fusion/support/tag_of.hpp>

namespace boost { namespace fusion
{
    struct random_access_traversal_tag;

    // Special tags:
    struct iterator_facade_tag; // iterator facade tag

    namespace extension
    {
        template <typename Tag>
        struct advance_impl
        {
            // default implementation
            template <typename Iterator, typename N>
            struct apply
              : mpl::if_c<
                    (N::value > 0)
                  , advance_detail::forward<Iterator, N::value>
                  , advance_detail::backward<Iterator, N::value>
                >::type
            {
                BOOST_FUSION_MPL_ASSERT_NOT(
                        traits::is_random_access<Iterator>,
            "default implementation not available for random access iterators");
            };
        };

        template <>
        struct advance_impl<iterator_facade_tag>
        {
            template <typename Iterator, typename N>
            struct apply
              : detail::remove_reference<Iterator>::type::template advance<Iterator, N>
            {};
        };
    }

    namespace result_of
    {
        template <typename Iterator, int N>
        struct advance_c
          : extension::advance_impl<typename traits::tag_of<Iterator>::type>::
                template apply<
                    typename detail::add_lref<Iterator>::type
                  , mpl::int_<N>
                >
        {};

        template <typename Iterator, typename N>
        struct advance
          : extension::advance_impl<typename traits::tag_of<Iterator>::type>::
                template apply<typename detail::add_lref<Iterator>::type, N>
        {};
    }

    template <int N, typename Iterator>
    inline typename result_of::advance_c<Iterator const&, N>::type const
    advance_c(Iterator const& i)
    {
        return result_of::advance_c<Iterator const&, N>::call(i);
    }

    template<typename N, typename Iterator>
    inline typename result_of::advance<Iterator const&, N>::type const
    advance(Iterator const& i)
    {
        return result_of::advance<Iterator const&, N>::call(i);
    }

}}

#endif
