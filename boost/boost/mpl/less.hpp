
#ifndef BOOST_MPL_LESS_HPP_INCLUDED
#define BOOST_MPL_LESS_HPP_INCLUDED

// Copyright (c) Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/bool.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/aux_/value_wknd.hpp>
#include <boost/mpl/aux_/na_spec.hpp>
#include <boost/mpl/aux_/lambda_support.hpp>

namespace boost {
namespace mpl {

namespace aux {

template<
      typename BOOST_MPL_AUX_NA_PARAM(T1)
    , typename BOOST_MPL_AUX_NA_PARAM(T2)
    >
struct less_c
{
    enum
    { 
        msvc71_wknd_ = ( BOOST_MPL_AUX_VALUE_WKND(T1)::value 
                        < BOOST_MPL_AUX_VALUE_WKND(T2)::value )
    };

    BOOST_STATIC_CONSTANT(bool, value = msvc71_wknd_);

#if !defined(__BORLANDC__)
    typedef bool_<value> type;
#else
    typedef bool_<(
          (BOOST_MPL_AUX_VALUE_WKND(T1)::value)
            < (BOOST_MPL_AUX_VALUE_WKND(T2)::value)
        )> type;
#endif
};

}

template<
      typename BOOST_MPL_AUX_NA_PARAM(T1)
    , typename BOOST_MPL_AUX_NA_PARAM(T2)
    >
struct less
{
    enum
    { 
        msvc71_wknd_ = ( BOOST_MPL_AUX_VALUE_WKND(T1)::value 
                        < BOOST_MPL_AUX_VALUE_WKND(T2)::value )
    };

    BOOST_STATIC_CONSTANT(bool, value = msvc71_wknd_);

#if !defined(__BORLANDC__)
    typedef bool_<value> type;
#else
    typedef bool_<(
          (BOOST_MPL_AUX_VALUE_WKND(T1)::value)
            < (BOOST_MPL_AUX_VALUE_WKND(T2)::value)
        )> type;
#endif

    BOOST_MPL_AUX_LAMBDA_SUPPORT(2,less,(T1,T2))
};

BOOST_MPL_AUX_NA_SPEC(2, less)

template< long N >
struct lt
{
    template< typename T > struct apply
#if !defined(__BORLANDC__)
        : less< T,integral_c<long,N> >
    {
#else
    {
        typedef typename less< T,integral_c<long,N> >::type type;
#endif
    };
};

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_LESS_HPP_INCLUDED
