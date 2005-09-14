// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_EXPONENT_HPP
#define BOOST_METAMATH_EXPONENT_HPP

#include <boost/mpl/tag.hpp>
#include <boost/mpl/apply.hpp>

namespace boost {namespace metamath {

template< typename Tag > struct exponent_impl;

template< typename T > struct exponent_tag
{
    typedef typename mpl::tag<T>::type type;
};

template<
      typename BOOST_MPL_AUX_NA_PARAM(N)
    >
struct exp 
    : mpl::apply1<
        exponent_impl<typename exponent_tag<N>::type>,
        N
    >::type
{
    BOOST_MPL_AUX_LAMBDA_SUPPORT(1, exp, (N))
};

}} //namespace boost::metamath

#include <boost/metamath/double/exponent.hpp>

#endif 
