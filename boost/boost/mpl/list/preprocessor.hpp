//-----------------------------------------------------------------------------
// boost mpl/list/preprocessor.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_LIST_PREPROCESSOR_HPP
#define BOOST_MPL_LIST_PREPROCESSOR_HPP

#include "boost/mpl/limits/list.hpp"

#define BOOST_MPL_LIST_PARAMETERS(param) \
    BOOST_MPL_TEMPLATE_PARAMETERS(0, BOOST_MPL_LIST_PARAMETERS_NUMBER, param) \
/**/

#define BOOST_MPL_LIST_DEFAULT_PARAMETERS(param, value) \
    BOOST_MPL_DEFAULT_TEMPLATE_PARAMETERS( \
          0 \
        , BOOST_MPL_LIST_PARAMETERS_NUMBER \
        , param \
        , value \
        ) \
/**/

#endif // BOOST_MPL_LIST_PREPROCESSOR_HPP
