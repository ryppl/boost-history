
#ifndef BOOST_MPL_PLUS_HPP_INCLUDED
#define BOOST_MPL_PLUS_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2003
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
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
      typename T, T N1, T N2, T N3 = 0, T N4 = 0, T N5 = 0
    >
struct plus_c
{
    BOOST_STATIC_CONSTANT(T, value = (N1 + N2 + N3 + N4 + N5));
#if !defined(__BORLANDC__)
    typedef integral_c<T,value> type;
#else
    typedef integral_c<T,(N1 + N2 + N3 + N4 + N5)> type;
#endif
};

template<
      typename BOOST_MPL_AUX_NA_PARAM(T1)
    , typename BOOST_MPL_AUX_NA_PARAM(T2)
    , typename T3 = integral_c<int,0>
    , typename T4 = integral_c<int,0>
    , typename T5 = integral_c<int,0>
    >
struct plus
    : plus_c<
          BOOST_MPL_AUX_TYPEOF(T1,
             T1::value + T2::value + T3::value + T4::value + T5::value
            )
        , BOOST_MPL_AUX_MSVC_VALUE_WKND(T1)::value
        , BOOST_MPL_AUX_MSVC_VALUE_WKND(T2)::value
        , BOOST_MPL_AUX_MSVC_VALUE_WKND(T3)::value
        , BOOST_MPL_AUX_MSVC_VALUE_WKND(T4)::value
        , BOOST_MPL_AUX_MSVC_VALUE_WKND(T5)::value
    >::type
{
    BOOST_MPL_AUX_LAMBDA_SUPPORT(5, plus, (T1,T2,T3,T4,T5))
};

BOOST_MPL_AUX_NA_SPEC2(2,5,plus)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_PLUS_HPP_INCLUDED
