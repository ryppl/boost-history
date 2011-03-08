//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_UTILITY_CONVERSION_NAME_LOOKUP_ER_2010_HPP
#define BOOST_ASSIGN_V2_UTILITY_CONVERSION_NAME_LOOKUP_ER_2010_HPP
#include <boost/assign/v2/utility/conversion/convert.hpp>
#include <boost/assign/v2/utility/conversion/converter.hpp>

// Expanding
//  namespace ns{
//   BOOST_ASSIGN_V2_CONVERTER( (A)(B)(C), range<A,B,C> )
//  }
// creates a name-lookup version of convert() and converter()  for name ns::range<A, B, C>


#define BOOST_ASSIGN_V2_UTILITY_CONVERSION_NAME_LOOKUP(Seq, R)\
BOOST_ASSIGN_V2_UTILITY_CONVERSION_CONVERT_NAME_LOOKUP(Seq, R)\
BOOST_ASSIGN_V2_UTILITY_CONVERSION_CONVERTER_NAME_LOOKUP(Seq, R)\
/**/

#endif
