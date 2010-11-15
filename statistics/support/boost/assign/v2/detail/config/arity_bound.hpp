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

// Default upper bound on the arity of function and functors
//#ifndef BOOST_ASSIGN_V2_ARITY_BOUND
//#define BOOST_ASSIGN_V2_ARITY_BOUND\
//	BOOST_PP_INC(BOOST_MPL_LIMIT_METAFUNCTION_ARITY)\
/**/    
//#endif

//#if BOOST_ASSIGN_V2_ARITY_BOUND >  BOOST_PP_INC(BOOST_MPL_LIMIT_METAFUNCTION_ARITY)
//#error
// This is required, at least, by /ref/fusion_matrix/container.hpp
//#endif

