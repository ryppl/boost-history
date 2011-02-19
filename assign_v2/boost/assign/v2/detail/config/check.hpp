//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHECK 
#include <boost/assert.hpp>
#define BOOST_ASSIGN_V2_CHECK( p ) BOOST_ASSERT( p )
#endif

// In the test suite, use, instead:
//    #include <boost/test/test_tools.hpp>
//    #define BOOST_ASSIGN_V2_CHECK( p ) BOOST_CHECK( p )
