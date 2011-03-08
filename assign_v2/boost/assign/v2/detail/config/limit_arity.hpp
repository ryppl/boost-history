//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/mpl/limits/arity.hpp>
#include <boost/assign/v2/detail/pp/parameter_list.hpp>

#ifndef BOOST_ASSIGN_V2_LIMIT_ARITY
//[limit_arity
/*<< n = BOOST_ASSIGN_V2_LIMIT_ARITY
   Overload                     | Arity
   -------------------------------------
   functor()                          |    0
   functor(x[0])                      |    1
   functor(x[0], x[1])                |    2
   ...
   functor(x[0], ...., x[n-1])        |    n
>>*/
#define BOOST_ASSIGN_V2_LIMIT_ARITY BOOST_MPL_LIMIT_METAFUNCTION_ARITY < BOOST_ASSIGN_V2_PARAMETER_LIST_SIZE ? BOOST_ASSIGN_V2_PARAMETER_LIST_SIZE : BOOST_MPL_LIMIT_METAFUNCTION_ARITY
//]

#else
// Meta-function classes associated with functor
#if BOOST_ASSIGN_V2_LIMIT_ARITY >  BOOST_MPL_LIMIT_METAFUNCTION_ARITY
#error
#endif

#endif // BOOST_ASSIGN_V2_LIMIT_ARITY

