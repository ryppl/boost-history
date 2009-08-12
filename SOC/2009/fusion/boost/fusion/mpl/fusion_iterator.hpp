/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_MPL_FUSION_ITERATOR_HPP
#define BOOST_FUSION_MPL_FUSION_ITERATOR_HPP

#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/prior.hpp>
#include <boost/fusion/iterator/advance.hpp>
#include <boost/fusion/iterator/distance.hpp>
#include <boost/fusion/support/category_of.hpp>

#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/advance_fwd.hpp>
#include <boost/mpl/distance_fwd.hpp>

namespace boost { namespace mpl
{
    template <typename It>
    struct fusion_iterator
    {
        typedef typename fusion::result_of::value_of<It>::type type;
        typedef typename fusion::traits::category_of<It>::type category;
        typedef It iterator;
    };

    //TODO deref

    template <typename It>
    struct next<fusion_iterator<It> >
    {
        typedef
            fusion_iterator<typename fusion::result_of::next<It>::type>
        type;
    };

    template <typename It>
    struct prior<fusion_iterator<It> >
    {
        typedef
            fusion_iterator<typename fusion::result_of::prior<It>::type>
        type;
    };

    template <typename It, typename N>
    struct advance<fusion_iterator<It>, N>
    {
        typedef
            fusion_iterator<
                typename fusion::result_of::advance<It, N>::type
            >
        type;
    };

    template <typename First, typename Last>
    struct distance<fusion_iterator<First>, fusion_iterator<Last> >
      : fusion::result_of::distance<First, Last>
    {};
}}

#endif
