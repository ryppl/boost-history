/*
 *
 * Copyright (c) 2004
 * John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         regex_raw_buffer.cpp
  *   VERSION      see <boost/version.hpp>
  *   DESCRIPTION: Member functions for class raw_storage.
  */


#define BOOST_REGEX_SOURCE
#include <memory>
#include <cstring>
#include <boost/assert.hpp>
#include <boost/regex/v4/regex_raw_buffer.hpp>

#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std{
   using ::memcpy;
   using ::memmove;
}
#endif


namespace boost{ namespace re_detail{

}} // namespaces
