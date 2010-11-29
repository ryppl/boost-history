//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
// TODO remove file
/*
#ifndef BOOST_ASSIGN_V2_PUT_RANGE_MODIFIER_DEDUCE_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_RANGE_MODIFIER_DEDUCE_ER_2010_HPP
#include <boost/assign/v2/detail/type_traits/container/has_static_size.hpp>
#include <boost/assign/v2/detail/type_traits/container/has_push.hpp>
#include <boost/assign/v2/detail/mpl/switch.hpp>
 
namespace boost{
namespace assign{
namespace v2{
namespace switch_tag{
	struct put_range{};
}// switch_tag
namespace put_range_tag{
	struct copy;
    struct put;
    struct range;
}// put_range_tag
namespace mpl{

    template<>
    struct case_<switch_tag::put_range,0> :
        v2::mpl::wrap<
        	put_range_tag::put,
            container_type_traits::has_push_deduced_value
        >{};

    template<>
    struct case_<switch_tag::put_range,1> :
        v2::mpl::wrap<
            v2::put_range_tag::copy,
            v2::container_type_traits::has_static_size
        >{};

    template<>
    struct case_<switch_tag::put_range,2> : 
        v2::mpl::wrap<put_range_tag::range>{};

}// mpl
namespace put_range_aux
{

    template<typename T>
    struct deduce_operation : v2::mpl::switch_<
    	v2::switch_tag::put_range,
    	T
    >{};
    
}// convert_aux
}// v2
}// assign
}// boost

#endif
*/