//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TYPE_TRAITS_CPP0X_ER_2010_HPP
#define BOOST_ASSIGN_V2_TYPE_TRAITS_CPP0X_ER_2010_HPP

#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>

#if BOOST_ASSIGN_V2_ENABLE_CPP0X

namespace boost{
namespace assign{
namespace v2{
namespace ref{

    // Maps T appearing in an rvalue reference T&& to the type appropriate
    // as the parameter of a reference wrapper.

	template<typename T>
    struct wrapper_param
    {
        // rvalue
    	typedef T const type;

    };

	template<typename T>
    struct wrapper_param<T&>
    {
    	typedef T type;
    };

	template<typename T>
    struct wrapper_param<T const &>
    {
    	typedef T const type;
    };


}// ref
}// v2
}// assign
}// boost

#endif

#endif // BOOST_ASSIGN_V2_TYPE_TRAITS_CPP0X_ER_2010_HPP
