//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_SUPPORT_TYPE_TRAITS_PARAM_ER_2011_HPP
#define BOOST_ASSIGN_V2_SUPPORT_TYPE_TRAITS_PARAM_ER_2011_HPP
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#include <boost/type_traits/add_const.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace type_traits{

#if BOOST_ASSIGN_V2_ENABLE_CPP0X

        template<typename T>
        struct param
            : boost::add_const<T>
        {
            // rvalue
        };

        // lvalue:

        template<typename T>
        struct param<T&>
        {
            typedef T type;
        };

#else

        template<typename T>
        struct param
        {
            typedef T type;
        };

#endif

}// type_traits
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_SUPPORT_TYPE_TRAITS_PARAM_ER_2011_HPP
