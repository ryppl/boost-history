//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TRAITS_CONTAINER_HAS_PUSH_ER_2010_HPP
#define BOOST_ASSIGN_V2_TRAITS_CONTAINER_HAS_PUSH_ER_2010_HPP
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/config.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>
#include <boost/assign/v2/detail/traits/container/value.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace container_traits{

    template<typename V,typename T
        = typename container_traits::value<V>::type>
    struct has_push{

        typedef typename boost::type_traits::yes_type yes_;
        typedef typename boost::type_traits::no_type no_;

        template<typename U,void (U::*fp)(T const&)>
        struct helper{
            // See http://lists.boost.org/Archives/boost/2002/03/27233.php
        };

        template<typename U>
        static yes_ test(U*, helper<U, &U::push>* p = 0);
        static no_ test(...);

        BOOST_STATIC_CONSTANT(
            bool,
            value = sizeof( test((V*)0) ) == sizeof( yes_ )
        );

        typedef ::boost::mpl::bool_<value> type;

    };

    template<typename V>
    struct has_push_deduced_value : v2::container_traits::has_push<V>{};

}// container_traits
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_TRAITS_CONTAINER_HAS_PUSH_ER_2010_HPP
