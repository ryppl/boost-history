//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_SUPPORT_TYPE_TRAITS_ADD_REF_CONST_ER_2011_HPP
#define BOOST_ASSIGN_V2_SUPPORT_TYPE_TRAITS_ADD_REF_CONST_ER_2011_HPP
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace type_traits{

    template<typename T>
    struct add_ref_const
        : boost::add_reference<
            typename boost::add_const<T>::type
        >
    {};

}// type_traits
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_SUPPORT_TYPE_TRAITS_ADD_REF_CONST_ER_2011_HPP
