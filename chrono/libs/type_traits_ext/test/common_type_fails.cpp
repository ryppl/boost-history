//  common_type_test.cpp  ----------------------------------------------------//

//  Copyright 2010 Beman Dawes

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#define _CRT_SECURE_NO_WARNINGS  // disable VC++ foolishness

#define STD2
#ifndef STD 
#include <boost/type_traits/common_type.hpp>
#else
#include <type_traits>
#endif
#include <iostream>


#include <boost/test/minimal.hpp>

struct C1 {
    //~ private:
        //~ C1();
};
    
struct C2 {};



#ifndef STD 
typedef boost::common_type<C1, C2>::type AC;
#else
typedef std::common_type<C1, C2>::type AC;
#endif    
//~ AC* ac;
   
    