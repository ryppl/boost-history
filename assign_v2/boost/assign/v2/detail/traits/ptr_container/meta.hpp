//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_TRAITS_PTR_CONTAINER_META_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_TRAITS_PTR_CONTAINER_META_ER_2010_HPP
#include <boost/assign/v2/detail/traits/ptr_container/to_value_container.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace container_aux{


    template<typename PtrC>
    struct is_ptr_container : ::boost::mpl::not_<
        boost::is_same<
            PtrC, typename to_value_container<PtrC>::type
        >
    >{};

    template<template<typename > class F>
    struct through_value_container
    {
        template<typename PtrC /*or C*/>
        struct apply : F<
            typename to_value_container<PtrC>::type
        >{};
    };

}// container_aux
}// v2
}// assign
}// boost

#endif //  BOOST_ASSIGN_V2_DETAIL_TRAITS_PTR_CONTAINER_META_ER_2010_HPP