// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_CONFIG_HAS_ADVICE_HPP_INCLUDED
#define BOOST_IDL_DETAIL_CONFIG_HAS_ADVICE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/interfaces/detail/config/is_flag.hpp>

#define BOOST_IDL_HAS_ADVICE() BOOST_IDL_IS_FLAG(BOOST_IDL_ENABLE_ADVICE)
    /**/

#endif // #ifndef BOOST_IDL_DETAIL_CONFIG_HAS_ADVICE_HPP_INCLUDED

