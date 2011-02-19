//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_CHECK_EQUAL_CONTAINER_DEDUCE_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_CHECK_EQUAL_CONTAINER_DEDUCE_ER_2010_HPP
#include <boost/assign/v2/detail/traits/container/is_fifo.hpp>
#include <boost/assign/v2/detail/traits/container/is_lifo.hpp>
#include <boost/assign/v2/detail/traits/container/is_map.hpp>
#include <boost/assign/v2/detail/traits/container/is_sorted.hpp>
#include <boost/assign/v2/detail/traits/container/is_array.hpp>
#include <boost/assign/v2/detail/traits/container/is_range.hpp>
#include <boost/assign/v2/detail/traits/switch.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace switch_tag{
    struct check_container{};
}// switch_tag
namespace switch_aux{

    template<>
    struct case_<switch_tag::check_container, 0> :
        switch_aux::helper<
            v2::container_tag::fifo,
            v2::container_traits::is_fifo
        >{};

    template<>
    struct case_<switch_tag::check_container, 1> :
        switch_aux::helper<
            v2::container_tag::lifo,
            v2::container_traits::is_lifo
        >{};

    // TODO remove bec. map is taken care of by sorted
    //template<>
    //struct case_<switch_tag::check_container, 2> :
    //    switch_aux::helper<
    //        v2::container_tag::map,
    //        v2::container_traits::is_map
    //    >{};

    template<>
    struct case_<switch_tag::check_container, 2> :
        switch_aux::helper<
            v2::container_tag::sorted,
            v2::container_traits::is_sorted
        >{};

    template<>
    struct case_<switch_tag::check_container, 3> :
        switch_aux::helper<
            v2::container_tag::array,
            v2::container_traits::is_array
        >{};

    template<>
    struct case_<switch_tag::check_container, 4> :
        switch_aux::helper<
            container_tag::range
        >{};

}// switch_aux
namespace check_aux{

    template<typename T>
    struct deduce_equal_container_tag : switch_aux::result<
        v2::switch_tag::check_container,
        T
    >
    {};

}// check_aux
}// v2
}// assign
}// boost

#endif
