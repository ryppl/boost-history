//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/mpl/limits/arity.hpp>

#ifndef BOOST_ASSIGN_V2_LIMIT_LVALUE_CONST_ARITY
#define BOOST_ASSIGN_V2_LIMIT_LVALUE_CONST_ARITY 2
#endif

// This constant defines the maximum arity for which functors are overloaded
// for any combination of lvalue and const under C++03 (the restriction does
// not apply to C++0x). Be warned, therefore, that the number of function 
// overloads increases geometrically with this number.  

#include <boost/assign/v2/detail/config/limit_arity.hpp>
#if BOOST_ASSIGN_V2_LIMIT_LVALUE_CONST_ARITY >  BOOST_ASSIGN_V2_LIMIT_ARITY
#error
#endif
