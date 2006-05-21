// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_SINE_HPP
#define BOOST_METAMATH_SINE_HPP

#include <boost/mpl/tag.hpp>
#include <boost/mpl/apply.hpp>
#include <math.h>

namespace boost {namespace metamath {

template< typename Tag > struct sine_impl;

template< typename T > struct sine_tag
{
    typedef typename mpl::tag<T>::type type;
};

template<
      typename BOOST_MPL_AUX_NA_PARAM(N)
    >
struct sine 
    : mpl::apply1<
        sine_impl<typename sine_tag<N>::type>,
        N
    >::type
{
    BOOST_MPL_AUX_LAMBDA_SUPPORT(1, sine, (N))
};

}} //namespace boost::metamath

#include <boost/metamath/double/sine.hpp>

#endif 

