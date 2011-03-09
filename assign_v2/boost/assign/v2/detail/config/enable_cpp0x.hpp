//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_ENABLE_CPP0X_ER_2010_HPP
#define BOOST_ASSIGN_V2_ENABLE_CPP0X_ER_2010_HPP

#ifndef BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <boost/preprocessor/logical/or.hpp>
#include <boost/config.hpp>
//[enable_cpp0x
#if defined(BOOST_NO_RVALUE_REFERENCES) || defined(BOOST_NO_VARIADIC_TEMPLATES) || defined(BOOST_NO_0X_HDR_TUPLE)
#define BOOST_ASSIGN_V2_ENABLE_CPP0X 0
#else
#define BOOST_ASSIGN_V2_ENABLE_CPP0X 1
#endif
//]
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X_ER_2010_HPP
