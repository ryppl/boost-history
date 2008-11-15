/*----------------------------------------------------------------------------+
Interval Template Library
Author: Joachim Faulhaber
Copyright (c) 2007-2008: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#include <iostream>
#include <string>
#include <math.h>
#include <boost/itl/type_traits/to_string.hpp>
#include <boost/itl/interval.hpp>
#include "../toytime.h"

using namespace std;
using namespace boost::itl;

/** Example interval.cpp \file interval.cpp

    Of course much of the library code deals with intervals which are implemented
    as class interval.

    This program gives a very short samlpe of different interval instances.

    \include example/interval/interval.cpp
*/
int main()
{
    cout << ">> Interval Template Library: Sample interval.cpp <<\n";
    cout << "----------------------------------------------------\n";

	interval<int>    int_Itv  = interval<int>::closed(3,7);
    interval<double> sqrt_Itv = interval<double>::rightopen(1/sqrt(2.0), sqrt(2.0));
    interval<string> city_Itv = interval<string>::leftopen("Barcelona", "Boston");
    interval<Time>   time_Itv = interval<Time>::open(Time(monday,8,30), Time(monday,17,20));

    cout << "Interval<int>: " << int_Itv.as_string() << endl;
    cout << "Interval<double>: " << sqrt_Itv.as_string() << " does " 
                                 << string(sqrt_Itv.contains(sqrt(2.0))?"":"NOT") 
                                 << " contain sqrt(2)" << endl;
    cout << "Interval<string>: " << city_Itv.as_string() << " does "  
                                 << string(city_Itv.contains("Barcelona")?"":"NOT") 
                                 << " contain 'Barcelona'" << endl;
    cout << "Interval<string>: " << city_Itv.as_string() << " does "  
                                 << string(city_Itv.contains("Berlin")?"":"NOT") 
                                 << " contain 'Berlin'" << endl;
    cout << "Interval<Time>: " << time_Itv.as_string() << endl;

    return 0;
}

// Program output:

// >> Interval Template Library: Sample interval.cpp <<
// ---------------------------------------------------
// Interval<int>: [3,7]
// Interval<double>: [0.707107,1.41421) does NOT contain sqrt(2)
// Interval<string>: (Barcelona,Boston] does NOT contain 'Barcelona'
// Interval<string>: (Barcelona,Boston] does  contain 'Berlin'
// Interval<Time>: (mon:08:30,mon:17:20)
