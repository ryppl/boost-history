//-----------------------------------------------------------------------------
// boost mpl/preprocessor/template_params.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_PREPROCESSOR_TEMPLATE_PARAMS_HPP
#define BOOST_MPL_PREPROCESSOR_TEMPLATE_PARAMS_HPP

#include "boost/mpl/preprocessor/config.hpp"
#include "boost/preprocessor/comma_if.hpp"
#include "boost/preprocessor/repeat.hpp"

#define BOOST_MPL_TEMPLATE_PARAMETER(i, param) \
    BOOST_PREPROCESSOR_COMMA_IF(i) param##i \
/**/

#define BOOST_MPL_TEMPLATE_PARAMS(param) \
    BOOST_PREPROCESSOR_REPEAT( \
        BOOST_MPL_PARAMETERS_NUMBER \
      , BOOST_MPL_TEMPLATE_PARAMETER \
      , param \
      ) \
/**/

#endif // BOOST_MPL_PREPROCESSOR_TEMPLATE_PARAMS_HPP
