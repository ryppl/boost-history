//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ARRAY_FUNCTOR_NTH_RESULT_OF_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ARRAY_FUNCTOR_NTH_RESULT_OF_ER_2010_HPP
#include <boost/assign/v2/ref/list/make.hpp>
#include <boost/assign/v2/ref/list/container.hpp>
#include <boost/assign/v2/ref/list/array.hpp>
#include <boost/assign/v2/ref/array/size_type.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace array_aux{

    template<typename T>
    struct empty_array : ref::empty_list<list_aux::array_tag>{};

    template<size_type N, typename T>
    struct recursive_result
    {
        typedef typename recursive_result<N-1, T>::type previous_;
        typedef typename previous_::template result<T&>::type type;
    };

    template<typename T>
    struct recursive_result<0, T> : empty_array<T>{};

}// array_aux
namespace nth_result_of{

    template<array_aux::size_type N, typename T>
    struct array
        : array_aux::recursive_result<N, T>
    {};

}// nth_result_of
}// ref
}// v2
}// assign
}// boost

#endif
