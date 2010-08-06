//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODIFIER_DEDUCE_ER_2010_HPP
#include <boost/assign/v2/detail/mpl/switch.hpp>

#include <boost/assign/v2/detail/type_traits/container/is_static_array.hpp>
#include <boost/assign/v2/detail/type_traits/container/has_push.hpp>
#include <boost/assign/v2/detail/type_traits/container/is_map.hpp>
 
namespace boost{
namespace assign{
namespace v2{
namespace switch_tag{
	struct deduce_put{};
}// switch_tag
namespace put_tag{
	struct at_next;
    struct insert;
    struct push;
    struct push_back;
}// put_tag
namespace mpl{

    template<>
    struct case_<switch_tag::deduce_put,0> :
        v2::mpl::wrap<
            v2::put_tag::insert,
            v2::container_type_traits::is_map
        >{};

    template<>
    struct case_<switch_tag::deduce_put,1> :
        v2::mpl::wrap<
            v2::put_tag::at_next,
            v2::container_type_traits::is_static_array
        >{};

    template<>
    struct case_<switch_tag::deduce_put,2> :
        v2::mpl::wrap<
        	put_tag::push,
            container_type_traits::has_push_deduced_value
        >{};

    template<>
    struct case_<switch_tag::deduce_put,3> : 
        v2::mpl::wrap<put_tag::push_back>{};

}// mpl
namespace put_aux{

    template<typename T>
    struct deduce_modifier : v2::mpl::switch_<
    	v2::switch_tag::deduce_put,
    	T
    >
    {};

	template<typename Tag> struct new_put_tag{};

}// put_aux
}// v2
}// assign
}// boost

#endif
