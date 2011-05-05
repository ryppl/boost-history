//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_SUPPORT_TYPE_TRAITS_OR_CONST_ER_2011_HPP
#define BOOST_ASSIGN_V2_SUPPORT_TYPE_TRAITS_OR_CONST_ER_2011_HPP
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <boost/mpl/or.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_const.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace type_traits{

template<typename ...Args> struct or_const 
    : ::boost::mpl::false_
{};

template<typename T, typename ...Args>
struct or_const<T, Args...> 
    : ::boost::mpl::or_<
        boost::is_const<T>,
        or_const<Args...>
    >
{};

template<typename T, typename ...Args>
struct or_const<T const, Args...> 
    : ::boost::mpl::true_
{};

}// type_traits
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
#endif // BOOST_ASSIGN_V2_SUPPORT_TYPE_TRAITS_OR_CONST_ER_2011_HPP
