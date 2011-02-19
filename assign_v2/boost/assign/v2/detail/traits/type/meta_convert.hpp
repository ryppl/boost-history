//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TRAITS_TYPE_HAS_VALUE_TYPE_ER_2010_HPP
#define BOOST_ASSIGN_V2_TRAITS_TYPE_HAS_VALUE_TYPE_ER_2010_HPP
//#include <boost/assign/v2/ref/convert_traits/result.hpp>

#include <boost/assign/v2/ref/convert_traits/reference.hpp>

namespace boost{
    struct boost::use_default;
namespace assign{
namespace v2{
namespace type_traits{

    template<typename Tag = boost::use_default>
    struct meta_convert{

        template<typename T1,typename T2>
//        struct apply : ref::convert_traits::result<T1, T2>{};
        struct apply : ref::convert_traits::reference<T1, T2>{};

    };

    // Specialize on Tag as needed.

}// type_traits
}// v2
}// assign
}// boost

#endif
