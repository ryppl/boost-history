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
#include <boost/assign/v2/detail/traits/value_container/category.hpp>
#include <boost/assign/v2/detail/check/equal_container/array.hpp>
#include <boost/assign/v2/detail/check/equal_container/fifo.hpp>
#include <boost/assign/v2/detail/check/equal_container/lifo.hpp>
#include <boost/assign/v2/detail/check/equal_container/range.hpp>
#include <boost/assign/v2/detail/check/equal_container/sorted.hpp>
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
            check_aux::fifo,
            container_aux::is_fifo
        >{};

    template<>
    struct case_<switch_tag::check_container, 1> :
        switch_aux::helper<
            check_aux::lifo,
            container_aux::is_lifo
        >{};

    template<>
    struct case_<switch_tag::check_container, 2> :
        switch_aux::helper<
            check_aux::sorted,
            container_aux::is_sorted
        >{};

    template<>
    struct case_<switch_tag::check_container, 3> :
        switch_aux::helper<
            check_aux::array,
            container_aux::is_array
        >{};

    template<>
    struct case_<switch_tag::check_container, 4> :
        switch_aux::helper<
            check_aux::range
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

#endif // BOOST_ASSIGN_V2_DETAIL_CHECK_EQUAL_CONTAINER_DEDUCE_ER_2010_HPP
