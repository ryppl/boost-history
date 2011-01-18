//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ARRAY_FUNCTOR_MAKE_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ARRAY_FUNCTOR_MAKE_ER_2010_HPP
#include <boost/assign/v2/ref/fusion/make.hpp>
#include <boost/assign/v2/ref/fusion/assign_array.hpp>
#include <boost/assign/v2/ref/array/functor/nth_result_of.hpp>
#include <boost/assign/v2/ref/array/functor/tag2.hpp>
#include <boost/assign/v2/ref/array/alloc/lazy.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace result_of{

	template<typename T>
    struct empty_array
    	: nth_result_of::array<0, T>{};

	template<typename T>
    struct array
    	: nth_result_of::array<1, T>{};

}// result_of

	template<typename T>
    typename result_of::empty_array<T>::type
	array( keyword_aux::nil const& )
    {
        typedef ref::array_tag1_ tag1_;
        typedef array_aux::functor_tag2_ tag2_;
    	return ref::fusion<tag1_, tag2_,T>( v2::_nil );
    }

    template<typename T>
	typename result_of::array<T>::type
    array(T& t)
    {
        return array<T>( v2::_nil )( t );
    }

    template<typename T>
	typename result_of::array<T const>::type
    array(T const & t)
    {
        return array<T const>( v2::_nil )( t );
    }

}// ref
}// v2
}// assign
}// boost

#endif
