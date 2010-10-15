/*-----------------------------------------------------------------------------+
Interval Container Library
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
/** Example interval.cpp \file interval.cpp
    \brief Intervals for integral and continuous instance types. 
           Closed and open interval borders.

    Much of the library code deals with intervals which are implemented
    as class interval. This program gives a very short samlpe of different 
    interval instances.

    \include interval_/interval.cpp
*/
//[example_interval
#include <iostream>
#include <string>
#include <math.h>
#include <boost/itl/discrete_interval.hpp>
#include <boost/itl/continuous_interval.hpp>
#include "../toytime.hpp"

using namespace std;
using namespace boost::icl;

int main()
{
    cout << ">>Interval Container Library: Sample interval.cpp <<\n";
    cout << "----------------------------------------------------\n";

    discrete_interval<int>      int_interval  
        = construct<discrete_interval<int> >(3, 7, interval_bounds::closed());
    continuous_interval<double> sqrt_interval 
        = construct<continuous_interval<double> >(1/sqrt(2.0), sqrt(2.0));
                                                 //interval_bounds::right_open() is default
    continuous_interval<string> city_interval 
        = construct<continuous_interval<string> >("Barcelona", "Boston", interval_bounds::left_open());

    discrete_interval<Time>     time_interval 
        = construct<discrete_interval<Time> >(Time(monday,8,30), Time(monday,17,20), 
                                              interval_bounds::open());

    cout << "  discrete_interval<int>:    " << int_interval  << endl;
    cout << "continuous_interval<double>: " << sqrt_interval << " does " 
                                            << string(contains(sqrt_interval, sqrt(2.0))?"":"NOT") 
                                            << " contain sqrt(2)" << endl;
    cout << "continuous_interval<string>: " << city_interval << " does "  
                                            << string(contains(city_interval,"Barcelona")?"":"NOT") 
                                            << " contain 'Barcelona'" << endl;
    cout << "continuous_interval<string>: " << city_interval << " does "  
                                            << string(contains(city_interval, "Berlin")?"":"NOT") 
                                            << " contain 'Berlin'" << endl;
    cout << "  discrete_interval<Time>:   " << time_interval << endl;

    return 0;
}

// Program output:

//>>Interval Container Library: Sample interval.cpp <<
//----------------------------------------------------
//  discrete_interval<int>:    [3,7]
//continuous_interval<double>: [0.707107,1.41421) does NOT contain sqrt(2)
//continuous_interval<string>: (Barcelona,Boston] does NOT contain 'Barcelona'
//continuous_interval<string>: (Barcelona,Boston] does  contain 'Berlin'
//  discrete_interval<Time>:   (mon:08:30,mon:17:20)
//]

