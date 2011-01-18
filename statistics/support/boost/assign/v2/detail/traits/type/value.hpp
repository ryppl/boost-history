//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TRAITS_TYPE_VALUE_ER_2010_HPP
#define BOOST_ASSIGN_V2_TRAITS_TYPE_VALUE_ER_2010_HPP
#include <boost/type_traits/remove_cv.hpp>
#include <boost/mpl/identity.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace type_traits{

    template<typename V>
    struct value_impl : ::boost::mpl::identity<
    	typename V::value_type
    >{};

    template<typename V>
    struct value : value_impl<
        typename boost::remove_cv<V>::type
    >{};

}// type_traits
}// v2
}// assign
}// boost

#endif
