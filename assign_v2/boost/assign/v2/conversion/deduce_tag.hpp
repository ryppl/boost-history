//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CONVERSION_DEDUCE_TAG_ER_2010_HPP
#define BOOST_ASSIGN_V2_CONVERSION_DEDUCE_TAG_ER_2010_HPP
#include <boost/assign/v2/support/switch.hpp>
#include <boost/assign/v2/support/traits/container.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/pair.hpp>

namespace boost{
    struct use_default;
namespace assign{
namespace v2{
namespace switch_tag{
    struct conversion{};
}// switch_tag

#define BOOST_ASSIGN_V2_SWITCH_TAG conversion
BOOST_ASSIGN_V2_SWITCH_CASE(0, container_aux::is_array, convert_tag::put)
BOOST_ASSIGN_V2_SWITCH_CASE(1, container_aux::is_multi_array, convert_tag::put)
BOOST_ASSIGN_V2_SWITCH_CASE(2, container_aux::has_push_deduced_value, convert_tag::put)
BOOST_ASSIGN_V2_SWITCH_CASE_DEFAULT(3, convert_tag::copy)
#undef BOOST_ASSIGN_V2_SWITCH_TAG

namespace conversion_aux{

    template<
        typename C        // Container
        , typename R    // Range
    >
    struct deduce_tag 
        : switch_aux::result<
            switch_tag::conversion,
            C
        >
    {};

}// conversion_aux
}// v2
}// assign
}// boost

#endif
