//  Boost string_algo library test test_main.cpp file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

// Include unit test framework
#include <boost/test/included/test_exec_monitor.hpp>

// Include tests
extern void trim_test();
extern void case_conv_test();
extern void string_predicate_test();

// test main 
int test_main( int, char*[] )
{
    trim_test();
    case_conv_test();
    string_predicate_test();

    return 0;
}
