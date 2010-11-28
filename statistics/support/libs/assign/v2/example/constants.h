//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_EXAMPLE_CONSTANTS_ER_2010_H
#define BOOST_ASSIGN_V2_EXAMPLE_CONSTANTS_ER_2010_H
#include <libs/assign/v2/example/foo.h>

namespace example_assign_v2{

//[foo_constants
namespace{

int a = 1, b = 5;

S const key_x =  "x" ;
S const key_y =  "y" ;
S const key_z =  "z" ;

const T x( a ); // (1, 0)
const T y; // (NULL, 0)
const T z( a, b ); //(1, 5)

}
//]


}// example_assign_v2

#endif
