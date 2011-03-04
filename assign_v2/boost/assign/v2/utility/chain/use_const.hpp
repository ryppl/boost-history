//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHAIN_USE_CONST_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHAIN_USE_CONST_ER_2010_HPP
#include <boost/config.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/range/reference.hpp>

namespace boost{
    struct use_default;
namespace assign{
namespace v2{
namespace chain_aux{

    template<typename U1, typename U2,
        bool is_r = boost::is_reference<U1>::value,
        bool is_c = boost::is_const<
            typename boost::remove_reference<U1>::type
        >::value
    >
    struct dont_use_const_impl : ::boost::mpl::false_{};

    template<typename U>
    struct dont_use_const_impl<U, U, true, false> : ::boost::mpl::true_{};

    template<typename R1, typename R2, typename Tag = use_default>
    struct dont_use_const : dont_use_const_impl<
        typename boost::range_reference<R1>::type,
        typename boost::range_reference<R2>::type
    >{};

    template<typename R1, typename R2, typename Tag = use_default>
    struct use_const : ::boost::mpl::bool_<
        !dont_use_const<R1, R2, Tag>::value
    >{};

}// chain_aux
}// v2
}// assign
}// boost

#endif
