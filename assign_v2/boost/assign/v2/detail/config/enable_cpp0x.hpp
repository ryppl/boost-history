//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <boost/preprocessor/logical/or.hpp>
#include <boost/config.hpp>
//[enable_cpp0x
#if defined(BOOST_NO_RVALUE_REFERENCES) || defined(BOOST_NO_VARIADIC_TEMPLATES) || defined(BOOST_NO_0X_HDR_TUPLE)
#define BOOST_ASSIGN_V2_ENABLE_CPP0X 0
#else
#define BOOST_ASSIGN_V2_ENABLE_CPP0X 1
#endif
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
//]

/*
 -- Previously,
//#if __GNUC__ && __GXX_EXPERIMENTAL_CXX0X__
//#define BOOST_ASSIGN_V2_ENABLE_CPP0X 1
//#else
//#define BOOST_ASSIGN_V2_ENABLE_CPP0X 0
//#endif

 -- Alternatively,

//#include <boost/preprocessor/logical/bool.hpp>
//#define BOOST_ASSIGN_V2_ENABLE_CPP0X BOOST_PP_BOOL(__cplusplus > 199711L)

(but GCC non-conformant.)

 -- Sources:
 http:gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html#Common-Predefined-Macros
 http:www2.research.att.com/~bs/C++0xFAQ.html
 http:msdn.microsoft.com/en-us/library/dd465215.aspx
*/
