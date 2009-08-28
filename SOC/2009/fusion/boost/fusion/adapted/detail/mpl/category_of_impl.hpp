/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_MPL_CATEGORY_OF_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_MPL_CATEGORY_OF_IMPL_HPP

//TODO doc

#include <boost/mpl/begin.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/iterator_category.hpp>
#include <boost/mpl/iterator_tags.hpp>

namespace boost { namespace fusion
{
    struct forward_traversal_tag;
    struct bidirectional_traversal_tag;
    struct random_access_traversal_tag;

    namespace detail
    {
        template <typename Category>
        struct mpl_iterator_category;

        template <>
        struct mpl_iterator_category<mpl::forward_iterator_tag>
        {
            typedef forward_traversal_tag type;
        };

        template <>
        struct mpl_iterator_category<mpl::bidirectional_iterator_tag>
        {
            typedef bidirectional_traversal_tag type;
        };

        template <>
        struct mpl_iterator_category<mpl::random_access_iterator_tag>
        {
            typedef random_access_traversal_tag type;
        };
    }

    namespace extension
    {
        template<typename>
        struct category_of_impl;

        template<>
        struct category_of_impl<mpl_sequence_tag>
        {
            template<typename Seq>
            struct apply
              : detail::mpl_iterator_category<
                    typename mpl::iterator_category<
                        typename mpl::begin<
                            typename detail::identity<Seq>::type
                        >::type
                    >::type
                >
            {};
        };

        template<>
        struct category_of_impl<mpl_iterator_tag>
        {
            template<typename It>
            struct apply
              : detail::mpl_iterator_category<
                    typename mpl::iterator_category<
                        typename detail::identity<It>::type
                    >::type
                >
            {};
        };
    }
}}

#endif
