//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_CONVERT_TRAITS_CONST_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONVERT_TRAITS_CONST_ER_2010_HPP
#include <boost/assign/v2/ref/convert_traits/value.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_cv.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace convert_traits{

    template<
        typename T1, typename T2, bool is_c
            = boost::is_const<T1>::value || boost::is_const<T2>::value
    >
    struct const_
    {
        typedef typename convert_traits::value<
            typename boost::remove_cv<T1>::type,
            typename boost::remove_cv<T2>::type
        >::type type;

    };

    template<typename T1, typename T2>
    struct const_<T1, T2, true> : boost::add_const<
        typename const_<T1, T2, false>::type
    >{};

}// convert_traits
}// ref
}// v2
}// assign
}// boost

#endif
