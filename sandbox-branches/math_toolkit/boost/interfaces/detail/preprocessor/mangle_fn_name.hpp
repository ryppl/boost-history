// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_MANGLE_FN_NAME_HPP_INCLUDED
#define BOOST_IDL_MANGLE_FN_NAME_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/preprocessor/cat.hpp>

// In the future, parameter names may be used for better mangling.
#define BOOST_IDL_MANGLE_FN_NAME(name, args) \
    BOOST_PP_CAT(name, BOOST_PP_CAT(_interface_, __LINE__)) \
    /**/

#endif // #ifndef BOOST_IDL_MANGLE_FN_NAME_HPP_INCLUDED
