
#ifndef BOOST_MPL_BITXOR_HPP_INCLUDED
#define BOOST_MPL_BITXOR_HPP_INCLUDED

// Copyright (c) Aleksey Gurtovoy 2000-2003
// Copyright (c) Jaap Suter 2000-2003
//
// Use, modification and distribution are subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy 
// at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/aux_/typeof.hpp>
#include <boost/mpl/aux_/value_wknd.hpp>
#include <boost/mpl/aux_/na_spec.hpp>
#include <boost/mpl/aux_/lambda_support.hpp>
#include <boost/config.hpp>

namespace boost {
namespace mpl {

template<
      typename T, T N1, T N2
    >
struct bitxor_c
{
    BOOST_STATIC_CONSTANT(T, value = (N1 ^ N2));
#if !defined(__BORLANDC__)
    typedef integral_c<T,value> type;
#else
    typedef integral_c<T,(N1 ^ N2)> type;
#endif
};

template<
      typename BOOST_MPL_AUX_NA_PARAM(T1)
    , typename BOOST_MPL_AUX_NA_PARAM(T2)
    >
struct bitxor_
    : bitxor_c<
          BOOST_MPL_AUX_TYPEOF(T1, T1::value ^ T2::value)
        , BOOST_MPL_AUX_MSVC_VALUE_WKND(T1)::value
        , BOOST_MPL_AUX_MSVC_VALUE_WKND(T2)::value
        >
{
    BOOST_MPL_AUX_LAMBDA_SUPPORT(2, bitxor_, (T1,T2))
};

BOOST_MPL_AUX_NA_SPEC(2,bitxor_)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_BITXOR_HPP_INCLUDED
