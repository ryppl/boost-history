////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                       //
//                                                                        //
//  Copyright (C) 2003-2004 Thorsten Ottosen                              //
//  Copyright (C) 2010 Erwann Rogard                                      //
//  Use, modification and distribution are subject to the                 //
//  Boost Software License, Version 1.0. (See accompanying file           //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)      //
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_PP_FORWARD_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_PP_FORWARD_ER_2010_HPP
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#define BOOST_ASSIGN_V2_FORWARD_PARAM(T, t) T&& t
#define BOOST_ASSIGN_V2_FORWARD_ARG(T, t) std::forward<T>( t )
#else
#define BOOST_ASSIGN_V2_FORWARD_PARAM(T, t) T& t
#define BOOST_ASSIGN_V2_FORWARD_ARG(T, t) t
#endif
#endif // BOOST_ASSIGN_V2_DETAIL_PP_FORWARD_ER_2010_HPP
