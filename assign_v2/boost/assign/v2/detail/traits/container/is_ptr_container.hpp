//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TRAITS_CONTAINER_IS_PTR_CONTAINER_ER_2010_HPP
#define BOOST_ASSIGN_V2_TRAITS_CONTAINER_IS_PTR_CONTAINER_ER_2010_HPP
#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/assign/v2/detail/traits/container/ptr_to_value.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace container_traits{

    template<typename V>
    struct is_ptr_container : ptr_to_value<V>::is_applicable{};

}// container_traits
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_TRAITS_CONTAINER_IS_PTR_CONTAINER_ER_2010_HPP
