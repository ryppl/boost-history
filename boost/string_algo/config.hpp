//  Boost string_algo library config.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_CONFIG_HPP
#define BOOST_STRING_CONFIG_HPP

#include <boost/config.hpp>

#ifdef BOOST_STRING_DEDUCED_TYPENAME
#   error "macro already defined!"
#endif

#define BOOST_STRING_TYPENAME BOOST_DEDUCED_TYPENAME

#endif  // BOOST_STRING_CONFIG_HPP
