//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TRAITS_VALUE_CONTAINER_HAS_VALUE_TYPE_ER_2010_HPP
#define BOOST_ASSIGN_V2_TRAITS_VALUE_CONTAINER_HAS_VALUE_TYPE_ER_2010_HPP
#include <boost/config.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace value_container_aux{

    template<typename T>
    struct has_value_type{

        typedef typename boost::type_traits::yes_type yes_;
        typedef typename boost::type_traits::no_type no_;


        template<typename U>
        static yes_ test(U*, typename U::value_type* p = 0);
        static no_ test(...);

        BOOST_STATIC_CONSTANT(
            bool,
            value = sizeof( test((T*)0) ) == sizeof( yes_ )
        );
        typedef ::boost::mpl::bool_<value> type;
    };


}// value_container_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_TRAITS_VALUE_CONTAINER_HAS_VALUE_TYPE_ER_2010_HPP
