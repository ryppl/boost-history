//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TRAITS_CONTAINER_PTR_TO_VALUE_CONTAINER_ER_2010_HPP
#define BOOST_ASSIGN_V2_TRAITS_CONTAINER_PTR_TO_VALUE_CONTAINER_ER_2010_HPP
#include <boost/assign/v2/detail/traits/container/is_ptr_container.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace container_traits{

    template<typename V>
    struct ptr_to_value_container{
        typedef typename ptr_to_value<V>::container_type type;
    };

}// container_traits
}// v2
}// assign
}// boost

#endif
