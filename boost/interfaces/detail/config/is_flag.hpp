// (C) Copyright Paul Mensonides 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_CONFIG_IS_FLAG_INCLUDED
#define BOOST_IDL_DETAIL_CONFIG_IS_FLAG_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/detail/is_nullary.hpp>

#define BOOST_IDL_IS_FLAG(id) \
    BOOST_PP_IS_NULLARY(BOOST_PP_CAT(BOOST_IDL_IS_FLAG_, id 1)) \
    /**/
#define BOOST_IDL_IS_FLAG_1 ()

#endif // #ifndef BOOST_IDL_DETAIL_CONFIG_IS_FLAG_INCLUDED
