/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ITERATOR_DETAIL_DISTANCE_HPP
#define BOOST_FUSION_ITERATOR_DETAIL_DISTANCE_HPP

#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/equal_to.hpp>

#include <boost/mpl/int.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/identity.hpp>

namespace boost { namespace fusion { namespace distance_detail
{
    // Default distance implementation, linear
    // search for the End iterator.

    template<typename Begin, typename End>
    struct linear_distance;

    template<typename Begin, typename End>
    struct next_distance
      : mpl::next<
            typename linear_distance<
                typename result_of::next<Begin>::type
              , End
            >::type
        >
    {};

    template<typename Begin, typename End>
    struct linear_distance
      : mpl::eval_if_c<
            result_of::equal_to<Begin, End>::value
          , mpl::identity<mpl::int_<0> >
          , next_distance<Begin, End>
        >::type
    {};

}}}

#endif
