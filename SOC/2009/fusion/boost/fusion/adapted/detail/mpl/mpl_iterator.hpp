/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_MPL_MPL_ITERATOR_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_MPL_MPL_ITERATOR_HPP

#include <boost/fusion/iterator/iterator_facade.hpp>
#include <boost/fusion/support/category_of.hpp>

#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/prior.hpp>
#include <boost/fusion/iterator/advance.hpp>
#include <boost/fusion/iterator/distance.hpp>

namespace boost { namespace fusion
{
    struct mpl_iterator_tag;
    struct mpl_sequence_tag;

    template <typename It>
    struct mpl_iterator
      : iterator_facade<
            mpl_iterator<It>
          , typename traits::category_of<It>::type
        >
    {
        template <typename ItRef>
        struct value_of
          : result_of::value_of<It>
        {};

        template <typename It1Ref, typename It2Ref>
        struct equal_to
          : result_of::equal_to<It, It2Ref>
        {};

        template <typename ItRef>
        struct deref
        {
            typedef typename result_of::deref<It>::type type;

            static type
            call(ItRef)
            {
                return type();
            }
        };

        template <typename ItRef>
        struct next
        {
            typedef typename result_of::next<It>::type type;

            static type
            call(ItRef)
            {
                return type();
            }
        };

        template <typename ItRef>
        struct prior
        {
            typedef typename result_of::prior<It>::type type;

            static type
            call(ItRef)
            {
                return type();
            }
        };

        template <typename ItRef, typename N>
        struct advance
        {
            typedef typename fusion::result_of::advance<It,N>::type type;

            static type
            call(ItRef)
            {
                return type();
            }
        };

        template <typename It1Ref, typename It2Ref>
        struct distance
          : result_of::distance<It, It2Ref>
        {};

        //cschmidt: void typedef to enable fast SFINAE in equal_to_impl.hpp!
        typedef void void_;
        typedef It it_type;
    };
}}

#endif
