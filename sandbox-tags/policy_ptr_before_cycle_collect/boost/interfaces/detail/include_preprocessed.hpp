
// NO INCLUDE GUARDS, THE HEADER IS INTENDED FOR MULTIPLE INCLUSION

// Copyright (C) Aleksey Gurtovoy 2000-2004
// Copyright (C) Jonathan Turkanis 2005
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

// Based on <boost/mpl/aux_/include_preprocessed.hpp>

#include <boost/interfaces/access.hpp>
#include <boost/mpl/aux_/config/preprocessor.hpp>
#include <boost/preprocessor/stringize.hpp>

#ifndef BOOST_IDL_NO_ACCESS_CONTROL
# define BOOST_IDL_COMPILER_DIR access_control
#else BOOST_IDL_COMPILER_DIR access_control
# define BOOST_IDL_COMPILER_DIR no_access_control
#endif

#define AUX_PREPROCESSED_HEADER \
    BOOST_IDL_COMPILER_DIR/BOOST_IDL_PREPROCESSED_HEADER \
    /**/
#include BOOST_PP_STRINGIZE(boost/interfaces/detail/preprocessed/AUX_PREPROCESSED_HEADER)
#undef AUX_PREPROCESSED_HEADER

#undef BOOST_IDL_PREPROCESSED_HEADER
