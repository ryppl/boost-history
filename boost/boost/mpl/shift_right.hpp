
#ifndef BOOST_MPL_SHIFT_RIGHT_HPP_INCLUDED
#define BOOST_MPL_SHIFT_RIGHT_HPP_INCLUDED

// Copyright (c) Aleksey Gurtovoy 2000-2003
// Copyright (c) Jaap Suter       2000-2003
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
      typename T, T N, typename ShiftT, ShiftT Shift
    >
struct shift_right_c
{
    BOOST_STATIC_CONSTANT(T, value = (N >> Shift));
#if !defined(__BORLANDC__)
    typedef integral_c<T,value> type;
#else
    typedef integral_c<T,(N >> Shift)> type;
#endif
};

template<
      typename BOOST_MPL_AUX_NA_PARAM(T)
    , typename BOOST_MPL_AUX_NA_PARAM(Shift)
    >
struct shift_right
    : shift_right_c<
          BOOST_MPL_AUX_TYPEOF(T, T::value)
        , BOOST_MPL_AUX_MSVC_VALUE_WKND(T)::value
        , BOOST_MPL_AUX_TYPEOF(Shift, Shift::value)
        , BOOST_MPL_AUX_MSVC_VALUE_WKND(Shift)::value
        >
{
    BOOST_MPL_AUX_LAMBDA_SUPPORT(2, shift_right, (T,Shift))
};

BOOST_MPL_AUX_NA_SPEC(2,shift_right)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_SHIFT_RIGHT_HPP_INCLUDED
