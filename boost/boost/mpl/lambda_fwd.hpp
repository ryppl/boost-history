
#ifndef BOOST_MPL_LAMBDA_FWD_HPP_INCLUDED
#define BOOST_MPL_LAMBDA_FWD_HPP_INCLUDED

// Copyright (c) Aleksey Gurtovoy 2001-2004
//
// Use, modification and distribution are subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy 
// at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/aux_/lambda_arity_param.hpp>
#include <boost/mpl/aux_/config/lambda.hpp>


#if !defined(BOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT)

#   include <boost/mpl/int.hpp>
#   include <boost/mpl/void.hpp>
#   include <boost/mpl/aux_/template_arity_fwd.hpp>

namespace boost { namespace mpl {

template< 
      typename T
    , typename Tag = void_
    BOOST_MPL_AUX_LAMBDA_ARITY_PARAM(
          typename Arity = int_< aux::template_arity<T>::value >
        )
    >
struct lambda;

}}

#else

namespace boost { namespace mpl {

template< 
      typename T
    , typename Tag
    , bool Protect
    > 
struct lambda;

}}

#endif

#endif // BOOST_MPL_LAMBDA_FWD_HPP_INCLUDED
