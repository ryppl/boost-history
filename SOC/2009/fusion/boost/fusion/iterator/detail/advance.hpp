/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ITERATOR_DETAIL_ADVANCE_HPP
#define BOOST_FUSION_ITERATOR_DETAIL_ADVANCE_HPP

#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/prior.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

namespace boost { namespace fusion { namespace detail
{
    // Default advance implementation, perform next(it)
    // or prior(it) N times.

    template <typename ItRef, int N>
    struct forward;

    template <typename ItRef, int N>
    struct next_forward
      : forward<
            typename result_of::next<ItRef>::type
          , N-1
        >
    {};

    template <typename ItRef, int N>
    struct forward
    {
        typedef typename
            mpl::eval_if_c<
                (N == 0)
              , mpl::identity<ItRef>
              , next_forward<ItRef, N>
            >::type
        type;

        static type
        call(type it)
        {
            return it;
        }

        template <typename OtherIt>
        static type
        call(OtherIt const& it)
        {
            return call(fusion::next(it));
        }
    };

    template <typename ItRef, int N>
    struct backward;

    template <typename ItRef, int N>
    struct next_backward
      : backward<
            typename result_of::prior<ItRef>::type
          , N+1
        >
    {};

    template <typename ItRef, int N>
    struct backward
    {
        typedef typename
            mpl::eval_if_c<
                (N == 0)
              , mpl::identity<ItRef>
              , next_backward<ItRef, N>
            >::type
        type;

        static type
        call(type it)
        {
            return it;
        }

        template <typename OtherIt>
        static type
        call(OtherIt const& it)
        {
            return call(fusion::prior(it));
        }
    };
}}}

#endif
