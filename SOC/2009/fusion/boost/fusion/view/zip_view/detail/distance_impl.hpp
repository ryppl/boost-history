/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_ZIP_VIEW_DETAIL_DISTANCE_IMPL_HPP
#define BOOST_FUSION_VIEW_ZIP_VIEW_DETAIL_DISTANCE_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/fusion/iterator/distance.hpp>
#include <boost/fusion/algorithm/query/find_if.hpp>
#include <boost/fusion/support/category_of.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    struct random_access_iterator_tag;

    namespace detail
    {
        template<typename FoundIt, typename SearchIt>
        struct best_distance
          : result_of::distance<
                FoundIt
              , typename result_of::find_if<
                    typename SearchIt::iterators
                  , is_same<
                        traits::category_of<mpl::_1>
                      , random_access_iterator_tag
                    >
                >::type
            >
        {};

        template<typename It1, typename It2>
        struct default_distance
          : result_of::distance<
                typename result_of::front<typename It1::iterators>::type
              , typename result_of::front<typename It2::iterators>::type
            >
        {};

        template<typename It1, typename It2>
        struct zip_view_iterator_distance
        {
            typedef typename
                result_of::find_if<
                    typename It1::iterators
                  , is_same<traits::category_of<mpl::_1>
                  , random_access_iterator_tag>
                >::type
            found_it;

            typedef typename
                mpl::eval_if<
                    is_same<
                        found_it
                      , typename result_of::end<typename It1::iterators>::type
                    >
                  , detail::default_distance<It1, It2>
                  , detail::best_distance<found_it, It2>
                >::type
            type;
        };
    }

    namespace extension
    {
        template<typename>
        struct distance_impl;

        template<>
        struct distance_impl<zip_view_iterator_tag>
        {
            template<typename It1, typename It2>
            struct apply
              : detail::zip_view_iterator_distance<
                    typename detail::remove_reference<It1>::type
                  , typename detail::remove_reference<It2>::type
                >
            {};
        };
    }
}}

#endif
