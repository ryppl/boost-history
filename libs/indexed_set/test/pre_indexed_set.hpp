/* Used in Boost.IndexedSet tests.
 *
 * Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_TEST_PRE_INDEXED_SET_HPP
#define BOOST_INDEXED_SET_TEST_PRE_INDEXED_SET_HPP

#define BOOST_INDEXED_SET_ENABLE_INVARIANT_CHECKING
#define BOOST_INDEXED_SET_ENABLE_SAFE_MODE

#include <boost/indexed_set/safe_mode_errors.hpp>

struct safe_mode_exception
{
  safe_mode_exception(boost::indexed_sets::safe_mode::error_code error_code_):
    error_code(error_code_)
  {}

  boost::indexed_sets::safe_mode::error_code error_code;
};

#define BOOST_INDEXED_SET_SAFE_MODE_ASSERT(expr,error_code) \
if(!(expr)){throw safe_mode_exception(error_code);}

#endif
