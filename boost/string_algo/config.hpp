//  Boost string_algo library config.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_CONFIG_HPP
#define BOOST_STRING_CONFIG_HPP

#include <boost/config.hpp>

// Compatibility issues

// Workaround for function call ambiguities
#ifdef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
#   define BOOST_STRING_MUTABLE_FUN(x) x##_mutable
#else
#   define BOOST_STRING_MUTABLE_FUN(x) x
#endif

#endif  // BOOST_STRING_CONFIG_HPP
