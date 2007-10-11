// test_manipulator_being_func.cpp

// Boost Logging library
//
// Author: John Torjo, www.torjo.com
//
// Copyright (C) 2007 John Torjo (see www.torjo.com for email)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.
 
// See http://www.boost.org for updates, documentation, and revision history.
// See http://www.torjo.com/log2/ for more details

#include <boost/logging/logging.hpp>

/** 
    Tests formatters and destinations being functions. In this case, calling them won't involve a virtual function call.
*/

using namespace boost::logging;


void test_manipulator_being_func() {
}

#ifdef SINGLE_TEST

int main() {
    test_manipulator_being_func();
}

#endif

// End of file

