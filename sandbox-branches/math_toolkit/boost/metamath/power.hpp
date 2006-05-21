// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_POWER_HPP
#define BOOST_METAMATH_POWER_HPP

#include <boost/mpl/tag.hpp>
#include <boost/mpl/apply.hpp>
#include <math.h>

namespace boost {namespace metamath {

template< typename Tag1,typename Tag2> struct power_impl;

template< typename T> struct power_tag
{
    typedef typename mpl::tag<T>::type type;
};

template<
      typename BOOST_MPL_AUX_NA_PARAM(N1),
      typename BOOST_MPL_AUX_NA_PARAM(N2)
    >
struct power 
    : mpl::apply2<
        power_impl<typename power_tag<N1>::type,typename power_tag<N2>::type>,
        N1,
        N2
    >::type
{
    BOOST_MPL_AUX_LAMBDA_SUPPORT(2, power, (N1,N2))
};

}} //namespace boost::metamath

#include <boost/metamath/double/power.hpp>

#endif 