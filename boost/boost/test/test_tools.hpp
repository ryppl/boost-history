//  test tools header  -------------------------------------------------------//

//  (C) Copyright Beman Dawes 2000. Permission to copy, use, modify, sell
//  and distribute this software is granted provided this copyright notice
//  appears in all copies. This software is provided "as is" without express or
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

//  Revision History
//    8 Apr 01  Use boost/test/unit_test.hpp as framework. (Beman)
//   26 Feb 01  Numerous changes suggested during formal review. (Beman)
//    7 Feb 01  #include <boost/test/test_main.cpp> if requested. (Beman)
//   22 Jan 01  Remove all header dependencies. (Beman)
//    3 Dec 00  Remove function definitions. (Ed Brey)
//    5 Nov 00  Initial boost version (Beman Dawes)

#ifndef BOOST_TEST_TOOLS_HPP
#define BOOST_TEST_TOOLS_HPP

//  for convenience, allow the user to request inclusion of lower-level layers
# ifdef BOOST_INCLUDE_MAIN
#  include <libs/test/src/test_main.cpp>
# else
#  include <boost/test/unit_test.hpp>   // for unit_test framework
# endif

#endif // BOOST_TEST_TOOLS_HPP
