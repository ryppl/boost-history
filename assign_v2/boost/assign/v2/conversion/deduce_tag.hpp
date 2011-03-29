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
#include <boost/assign/v2/detail/traits/switch.hpp>
#include <boost/assign/v2/detail/traits/container.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/pair.hpp>

namespace boost{
    struct use_default;
namespace assign{
namespace v2{
namespace switch_tag{
    struct deduce_convert{};
}// switch_tag
namespace conversion_aux{

    // This is in replacement of switch_aux::helper since here we need
    // two arguments.

    template<typename T, typename U>
    struct default_f : ::boost::mpl::true_{};

    template<typename Tag,
        template<typename, typename> class F = conversion_aux::default_f>
    struct helper
    {
        typedef Tag tag;
        template<typename T>  // T must derive from mpl::pair<>
        struct apply
            : F<typename T::first, typename T::second>
        {
        };
    };

    template<typename C, typename R>
    struct use_put : ::boost::mpl::or_<
        container_aux::is_array<C>,
        container_aux::has_push_deduced_value<C>
    >{};

}// conversion_aux
namespace switch_aux{

    template<>
    struct case_<switch_tag::deduce_convert, 0> :
        conversion_aux::helper<
            v2::convert_tag::put,
            v2::conversion_aux::use_put
        >{};

    template<>
    struct case_<switch_tag::deduce_convert, 1> :
        conversion_aux::helper<v2::convert_tag::copy>{};

}// switch_aux
namespace conversion_aux{

    template<typename C, typename R>
    struct deduce_tag : v2::switch_aux::result<
        v2::switch_tag::deduce_convert,
        boost::mpl::pair<C, R>
    >
    {};

}// conversion_aux
}// v2
}// assign
}// boost

#endif
