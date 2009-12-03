/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#include <iostream>
#include <stdio.h>

#include <boost/validate/driver/sorted_associative_map_back_driver.hpp>

using namespace std;
using namespace Loki;
using namespace boost;
using namespace boost::itl;

void test_sorted_associative_map_back_driver()
{
    sorted_associative_map_back_driver validater;
    cout << 
    ">> ------------------------------------------------------ <<\n"
    ">> -------- Law based test automaton 'LaBatea' ---------- <<\n";

#if(_MSC_VER >= 1400 && defined(_DEBUG) ) // 1500=MSVC-9.0 1400=MSVC-8.0; 1310=MSVC-7.1; 1300=MSVC-7.0; 
    cout << msvc_stl_false_order_checking_for_reverse_iteration_message();
#else
    cout <<
    ">> Output will be generated in a few seconds\n"
    ">> terminate by typing <CTRL>C\n"
    ">> ------------------------------------------------------ <<\n";
    GentorProfileSgl::it()->set_std_profile(16,1);
    GentorProfileSgl::it()->report_profile();
    validater.validate();
#endif

};


int main()
{
    test_sorted_associative_map_back_driver();
    return 0;
}
