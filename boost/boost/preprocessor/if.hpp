#ifndef BOOST_PREPROCESSOR_IF_HPP
#define BOOST_PREPROCESSOR_IF_HPP

// Copyright (C) 2001
// Housemarque Oy
// http://www.housemarque.com
//
// Permission to copy, use, modify, sell and distribute this software is
// granted provided this copyright notice appears in all copies. This
// software is provided "as is" without express or implied warranty, and
// with no claim as to its suitability for any purpose.

// See http://www.boost.org for most recent version.

/*! \file

<a href="../../../../boost/preprocessor/if.hpp">Click here to see the header.</a>
*/

#include <boost/preprocessor/logical/bool.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

//! Expands to T if C != 0 and E if C == 0.
/*!
BOOST_PP_IF() enables convenient generation of lists using
BOOST_PP_REPEAT().
*/
#define BOOST_PP_IF(C,T,E) BOOST_PP_IF_BOOL(BOOST_PP_BOOL(C))(E,T)

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define BOOST_PP_IF_BOOL(C) BOOST_PP_IF_BOOL_DELAY(C)
#define BOOST_PP_IF_BOOL_DELAY(C) BOOST_PP_TUPLE2_ELEM##C
#endif

//! Obsolete. Use BOOST_PP_IF().
#define BOOST_PREPROCESSOR_IF(C,T,E) BOOST_PP_IF(C,T,E)
#endif
