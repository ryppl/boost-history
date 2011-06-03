//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ARRAY_ARRAY_ER_2011_HPP
#define BOOST_ASSIGN_V2_REF_ARRAY_ARRAY_ER_2011_HPP
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/assign/v2/ref/array/alloc/lazy.hpp>
#include <boost/assign/v2/ref/array/size_type.hpp>
#include <boost/assign/v2/ref/aux_/list/list.hpp>
#include <boost/assign/v2/ref/aux_/list/array.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_ref_array_array
namespace ref{
//<-
namespace array_aux{

    template<typename T>
    struct empty_array
        : empty_list<list_aux::array_tag>
    {};

    template<size_type N, typename T>
    struct recursive_result
    {
        typedef typename recursive_result<N-1, T>::type previous_;
        typedef typename previous_::template result<T&>::type type;
    };

    template<typename T>
    struct recursive_result<0, T>
        : empty_array<T>
    {};

}// array_aux
//->
namespace nth_result_of{

    template<array_size_type N, typename T>
    struct array/*<-*/
        : array_aux::recursive_result<N, T>
    {}/*->*/;

}// nth_result_of
namespace result_of{

    template<typename U, typename T = U>
    struct array/*<-*/
        : nth_result_of::array<1, T>
    {}/*->*/;

    template<typename U>
    struct array<U, kwd_nil_>/*<-*/
        : nth_result_of::array<0, U>
    {}/*->*/;

}// result_of

    // Generates an empty array
    template<typename T>
    typename result_of::array<T, kwd_nil_>::type
    array( kwd_nil_ )/*<-*/
    {
        return ref::list<list_aux::array_tag>( v2::_nil );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    // Generates a size-one array
    template<typename T>
    typename result_of::array<T>::type
    array(T& t)/*<-*/
    {
        return array<T>( v2::_nil )( t );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    // Generates a size-one array
    template<typename T>
    typename result_of::array<T const>::type
    array(T const & t)/*<-*/
    {
        return array<T const>( v2::_nil )( t );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

}// ref
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_ARRAY_ARRAY_ER_2011_HPP
