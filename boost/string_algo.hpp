//  Boost string_algo library string_algo.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_ALGO_HPP
#define BOOST_STRING_ALGO_HPP

#include <boost/config.hpp>

// Compatibility issues

// Workaround for function call ambiguities
#ifdef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
#   define BOOST_STRING_NON_CONST_FUNCTION(x) x##_nc
#else
#   define BOOST_STRING_NON_CONST_FUNCTION(x) x
#endif

#include <boost/string_algo/string_funct.hpp>
#include <boost/string_algo/string_trim.hpp>
#include <boost/string_algo/string_conv.hpp>
#include <boost/string_algo/string_predicate.hpp>
#include <boost/string_algo/string_substr.hpp>

#endif  // BOOST_STRING_ALGO_HPP
