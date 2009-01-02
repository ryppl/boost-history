/*----------------------------------------------------------------------------+
Interval Template Library
Author: Joachim Faulhaber
Copyright (c) 2007-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#include <boost/validate/gentor/gentorprofile.h>

using namespace boost::itl;

GentorProfileSgl* GentorProfileSgl::s_instance = NULL;

GentorProfileSgl* GentorProfileSgl::it()
{
    if(s_instance==NULL) {
        s_instance = new GentorProfileSgl;
    }
    return s_instance;
}

GentorProfileSgl::GentorProfileSgl(){}



GentorProfile::GentorProfile()
{
    //---------------------------------
    //standard values
    //set_range_int(-10, 10);
    //set_range_nat(0, 64);
    //set_range_unsigned_short(0, 64);
    //set_range_double(0.0, 1.0);
    //set_range_ContainerSize(0,10);

    //set_range_interval_int(-10, 10);
    //set_maxIntervalLength(8);

    //set_range_element_ContainerSize(0,5);

    //---------------------------------
    //small values
    set_range_int(0, 10);
    set_range_nat(0, 16);
    set_range_double(0.0, 1.0);
    set_range_ContainerSize(0,4);

    set_range_interval_int(0, 10);
    set_maxIntervalLength(5);
    set_range_element_ContainerSize(0,4);

    //---------------------------------
    //current values
    //set_range_int(-5, 5);
    //set_range_nat(0, 16);
    //set_range_double(0.0, 1.0);
    //set_range_ContainerSize(0,6);

    //set_range_interval_int(-5, 5);
    //set_maxIntervalLength(6);

    //set_range_element_ContainerSize(0,4);
}

// -------------------------------------
// (Algebra, Law, Type) -> (testsCount)
// (set, ipAddCommut, itl::set) -> 150

