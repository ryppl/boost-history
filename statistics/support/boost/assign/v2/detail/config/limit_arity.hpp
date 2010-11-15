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

#ifndef BOOST_ASSIGN_V2_LIMIT_ARITY
#define BOOST_ASSIGN_V2_LIMIT_ARITY BOOST_MPL_LIMIT_METAFUNCTION_ARITY
#endif

#if BOOST_ASSIGN_V2_LIMIT_ARITY_BOUND >  BOOST_MPL_LIMIT_METAFUNCTION_ARITY
#error
// This is required for a metafunction class that returns the result type of a 
// functors.
#endif

// Functors are by default overloaded as follows:
// 
// n = BOOST_ASSIGN_V2_LIMIT_ARITY
// Overload                     | Arity
// -------------------------------------
// f()                          |	0
// f(x[0])                      |	1
// f(x[0], x[1])                |	2
// ...
// f(x[0], ...., x[n-1])        |	n

