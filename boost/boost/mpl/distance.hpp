
#ifndef BOOST_MPL_DISTANCE_HPP_INCLUDED
#define BOOST_MPL_DISTANCE_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/distance_fwd.hpp>
#include <boost/mpl/iter_fold.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/tag.hpp>
#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/aux_/msvc_eti_base.hpp>
#include <boost/mpl/aux_/na_spec.hpp>

namespace boost { namespace mpl {

// default implementation for forward/bidirectional iterators
template< typename Tag > struct distance_impl
{
    template< typename First, typename Last > struct apply
        : aux::msvc_eti_base< typename iter_fold<
              iterator_range<First,Last>
            , long_<0>
            , next<>
            >::type >
    {
    };
};

template<
      typename BOOST_MPL_AUX_NA_PARAM(First)
    , typename BOOST_MPL_AUX_NA_PARAM(Last)
    >
struct distance
    : apply_wrap2<
          distance_impl< typename tag<First>::type >
        , First
        , Last
        >
{
};

// BOOST_MPL_AUX_NA_SPEC(2, distance)

}}

#endif // BOOST_MPL_DISTANCE_HPP_INCLUDED
