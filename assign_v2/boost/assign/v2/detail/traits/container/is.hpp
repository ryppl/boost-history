//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TRAITS_CONTAINER_IS_ER_2010_HPP
#define BOOST_ASSIGN_V2_TRAITS_CONTAINER_IS_ER_2010_HPP
#include <boost/assign/v2/detail/traits/container/fwd.hpp>
#include <boost/assign/v2/detail/traits/container/forward_to_value.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/type_traits/remove_cv.hpp>
#define BOOST_ASSIGN_V2_TRAITS_CONTAINER_IS(name)\
namespace container_traits{\
    template<typename V>\
    struct BOOST_PP_CAT(is_,name) : forward_to_value<\
        container_traits_aux::BOOST_PP_CAT(is_,name),\
        typename boost::remove_cv<V>::type\
    >{};\
}\
/**/
#endif // BOOST_ASSIGN_V2_TRAITS_CONTAINER_IS_ER_2010_HPP
