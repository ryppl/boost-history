//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_LIMIT_ARITY_ER_2010_HPP
#define BOOST_ASSIGN_V2_LIMIT_ARITY_ER_2010_HPP
#include <boost/assign/v2/detail/pp/parameter_list.hpp>
#include <boost/mpl/limits/arity.hpp>
#include <boost/preprocessor/comparison/greater.hpp>
#include <boost/preprocessor/selection/min.hpp>
#ifndef BOOST_ASSIGN_V2_PARAMETER_LIST_SIZE
#error
#endif
#ifndef BOOST_ASSIGN_V2_LIMIT_ARITY
//[limit_arity
/* n = BOOST_ASSIGN_V2_LIMIT_ARITY
   Overload                           | Arity
   -------------------------------------
   functor()                          |    0
   functor(x[0])                      |    1
   functor(x[0], x[1])                |    2
   ...
   functor(x[0], ...., x[n-1])        |    n
*/
#define BOOST_ASSIGN_V2_LIMIT_ARITY BOOST_PP_MIN( BOOST_MPL_LIMIT_METAFUNCTION_ARITY, BOOST_ASSIGN_V2_PARAMETER_LIST_SIZE )
//]
#endif // BOOST_ASSIGN_V2_LIMIT_ARITY
//[limit_arity_requirement
/*Required for some meta-function classes yielding result type of invoking a functor*/  
#if BOOST_PP_GREATER( BOOST_MPL_LIMIT_METAFUNCTION_ARITY, BOOST_ASSIGN_V2_LIMIT_ARITY )
#error
#endif
//]
#endif // BOOST_ASSIGN_V2_LIMIT_ARITY_ER_2010_HPP