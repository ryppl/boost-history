//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_LIMIT_LVALUE_CONST_ARITY_ER_2010_HPP
#define BOOST_ASSIGN_V2_LIMIT_LVALUE_CONST_ARITY_ER_2010_HPP
#include <boost/assign/v2/detail/config/limit_arity.hpp>
#include <boost/preprocessor/comparison/greater.hpp>
//[limit_lvalue_const_arity
/*<<
   Maximum arity for which functors are overloaded for any combination of const 
   and non-const lvalues under C++03. Be warned that the number of function
   overloads increases geometrically with this number.
>>*/
#ifndef BOOST_ASSIGN_V2_LIMIT_LVALUE_CONST_ARITY
#define BOOST_ASSIGN_V2_LIMIT_LVALUE_CONST_ARITY 2
#endif
//]
//[limit_lvalue_const_arity_requirement
#if BOOST_PP_GREATER( BOOST_ASSIGN_V2_LIMIT_LVALUE_CONST_ARITY,  BOOST_ASSIGN_V2_LIMIT_ARITY )
#error
#endif
//]
#endif // BOOST_ASSIGN_V2_LIMIT_LVALUE_CONST_ARITY_ER_2010_HPP