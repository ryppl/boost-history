//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_DEDUCE_MODIFIER_TAG_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_DEDUCE_MODIFIER_TAG_ER_2010_HPP
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/assign/v2/detail/traits/switch.hpp>
#include <boost/assign/v2/detail/traits/container/is_array.hpp>
#include <boost/assign/v2/detail/traits/container/has_push.hpp>
#include <boost/assign/v2/detail/traits/container/is_associative.hpp>

#include <boost/assign/v2/put/deduce/fwd.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace switch_tag{
    struct deduce_put{};
}// switch_tag
namespace switch_aux{

    template<>
    struct case_<switch_tag::deduce_put, 0> :
        switch_aux::helper<
            v2::modifier_tag::insert,
            v2::container_traits::is_associative
        >{};

    template<>
    struct case_<switch_tag::deduce_put, 1> :
        switch_aux::helper<
            v2::modifier_tag::iterate,
            v2::container_traits::is_array
        >{};

    template<>
    struct case_<switch_tag::deduce_put, 2> :
        switch_aux::helper<
            v2::modifier_tag::push,
            container_traits::has_push_deduced_value
        >{};

    template<>
    struct case_<switch_tag::deduce_put, 3> :
        switch_aux::helper<v2::modifier_tag::push_back>{};

}// switch_aux
namespace put_aux{

    template<typename T>
    struct deduce_modifier_tag
        : switch_aux::result< switch_tag::deduce_put, T>
    {};

    // For testing purposes
    template<typename C, typename X>
    void check_deduce()
    {
        typedef typename put_aux::deduce_modifier_tag<C>::type found_;
        BOOST_MPL_ASSERT(( boost::is_same<found_, X> ));
    }

}// put_aux
}// v2
}// assign
}// boost

#endif
