//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ARRAY_CONVERSION_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ARRAY_CONVERSION_ER_2010_HPP
#include <boost/assign/v2/ref/array/interface.hpp>
#include <boost/assign/v2/conversion/converter.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace array_aux{

#define BOOST_ASSIGN_V2_SEQ (Impl)(D)
#define BOOST_ASSIGN_V2_R interface<Impl, D>
BOOST_ASSIGN_V2_CONVERSION_CONVERTER_NAME_LOOKUP(BOOST_ASSIGN_V2_SEQ, BOOST_ASSIGN_V2_R)
#undef BOOST_ASSIGN_V2_SEQ
#undef BOOST_ASSIGN_V2_R

}// array_aux
}// ref
}// v2
}// assign
}// boost

#endif
