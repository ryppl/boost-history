//  Boost string_algo library config.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_CONFIG_HPP
#define BOOST_STRING_CONFIG_HPP

#include <boost/config.hpp>

// Config library defines BOOST_NO_DEDUCED_TYPENAME for VC++ 6.0 only
// The bug is however also present in VC++7.0

#ifdef BOOST_MSVC
#   if BOOST_MSVC <= 1300  // 1300 == VC++ 7.0
#       define BOOST_STRING_DEDUCED_TYPENAME 
#   else
#       define BOOST_STRING_DEDUCED_TYPENAME BOOST_DEDUCED_TYPENAME
#   endif
#else
#   define BOOST_STRING_DEDUCED_TYPENAME BOOST_DEDUCED_TYPENAME
#endif

#endif  // BOOST_STRING_CONFIG_HPP
