//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TRAITS_CONTAINER_FWD_TO_VALUE_ER_2010_HPP
#define BOOST_ASSIGN_V2_TRAITS_CONTAINER_FWD_TO_VALUE_ER_2010_HPP
#include <boost/assign/v2/detail/traits/container/value_container.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace container_traits{

	template<template<typename T> class F,typename V>
    struct forward_to_value: F<
    	typename value_container<V>::type
    >{};

}// container_traits
}// v2
}// assign
}// boost

#endif
