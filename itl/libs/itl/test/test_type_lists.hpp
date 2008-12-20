/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#ifndef __itl_test_type_lists_JOFA_080916_H__
#define __itl_test_type_lists_JOFA_080916_H__

#include <boost/mpl/list.hpp>

// interval instance types
#include <boost/itl/gregorian.hpp> 
#include <boost/itl/ptime.hpp> 
#include <boost/itl/rational.hpp> 

#include <boost/itl/interval.hpp>


typedef ::boost::mpl::list<
    unsigned short, unsigned int, unsigned long  
    ,short, int, long
    ,float, double
    ,boost::rational<int>
    ,boost::posix_time::ptime
//    ,boost::gregorian::date
> bicremental_types;

//DBG short list for debugging
//typedef ::boost::mpl::list<
//    int
//> bicremental_types;

typedef ::boost::mpl::list<
    float, double
    ,boost::rational<int>
> bicremental_continuous_types;

typedef ::boost::mpl::list<
    unsigned short, unsigned int, unsigned long  
    ,short, int, long
> integral_types;

typedef ::boost::mpl::list<
    unsigned short, unsigned int, unsigned long  
    ,short, int, long
    ,boost::posix_time::ptime
//    ,boost::gregorian::date
> discrete_types;

typedef ::boost::mpl::list<
    float, double
    ,boost::rational<int>
    ,std::string
> continuous_types;

typedef ::boost::mpl::list<
    unsigned short, unsigned int, unsigned long  
    ,short, int, long
    ,float, double
    ,boost::rational<int>
    ,std::string
    ,boost::posix_time::ptime
//    ,boost::gregorian::date
> ordered_types;

#endif 

