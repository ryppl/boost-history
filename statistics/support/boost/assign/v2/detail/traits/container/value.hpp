//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TRAITS_CONTAINER_VALUE_ER_2010_HPP
#define BOOST_ASSIGN_V2_TRAITS_CONTAINER_VALUE_ER_2010_HPP
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/assign/v2/detail/traits/type/value.hpp>
#include <boost/assign/v2/detail/traits/container/forward_to_value.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace container_traits
{

	// TODO
    // maybe this should be
    // 	remove_reference<V::reference>::type
    // or 
    // 	range_value<V>, is available.
    // Think about ptr_container in making this change.
    template<typename V>
    struct value : container_traits::forward_to_value<
    	type_traits::value,
        V
    >{};

}// container_traits
}// v2
}// assign
}// boost

#endif
