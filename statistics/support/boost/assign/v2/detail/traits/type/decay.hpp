//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TRAITS_TYPE_DECAY_OF_ER_2010_HPP
#define BOOST_ASSIGN_V2_TRAITS_TYPE_DECAY_OF_ER_2010_HPP
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/decay.hpp>
#include <boost/type_traits/add_const.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace type_traits{

    // To support string literals properly
    template< class T >
    struct decay : boost::mpl::eval_if<
        ::boost::is_array<T>,
        ::boost::decay<const T>,
        ::boost::decay<T> >
    {};

}// type_traits
}// v2
}// assign
}// boost

#endif
