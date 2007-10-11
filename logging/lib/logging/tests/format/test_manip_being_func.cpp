// test_manipulator_being_func.cpp

// Boost Logging library
//
// Author: John Torjo, www.torjo.com
//
// Copyright (C) 2007 John Torjo (see www.torjo.com for email)
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
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

