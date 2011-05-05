//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_CONVERT_TRAITS_REFERENCE_ER_2011_HPP
#define BOOST_ASSIGN_V2_REF_CONVERT_TRAITS_REFERENCE_ER_2011_HPP
#include <boost/assign/v2/ref/aux_/convert_traits/const.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace convert_traits{

    template<typename U1, typename U2>
    struct reference : convert_traits::const_<
        typename boost::remove_reference<U1>::type,
        typename boost::remove_reference<U2>::type
    >{};

    template<typename T>
    struct reference<T&, T&>{ typedef T& type; };

    template<typename T>
    struct reference<T const&, T&>{ typedef T const& type; };

    template<typename T>
    struct reference<T &, T const&>{ typedef T const& type; };

}// convert_traits
}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_CONVERT_TRAITS_REFERENCE_ER_2011_HPP

