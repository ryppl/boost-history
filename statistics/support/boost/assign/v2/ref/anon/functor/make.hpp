//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ANON_FUNCTOR_MAKE_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ANON_FUNCTOR_MAKE_ER_2010_HPP
#include <boost/assign/v2/ref/fusion/make.hpp>
#include <boost/assign/v2/ref/static_array/alloc/lazy_alloc.hpp>
#include <boost/assign/v2/ref/fusion/assign_array.hpp>
#include <boost/assign/v2/ref/anon/functor/nth_result_of.hpp>
#include <boost/assign/v2/ref/anon/functor/tag2.hpp>
 
namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace result_of{

	template<typename Tag1,typename T>
    struct empty_anon 
    	: nth_result_of::anon<0, Tag1, T>{};

	template<typename Tag1, typename T>
    struct anon 
    	: nth_result_of::anon<1, Tag1, T>{};

}// result_of

	template<typename Tag1, typename T>
    typename result_of::empty_anon<Tag1, T>::type
	anon( keyword_aux::nil const& )
    {
    	typedef anon_aux::unary_tag2_ tag2_;
    	return ref::fusion<Tag1, tag2_,T>( v2::_nil );
    }

    template<typename Tag1,typename T>
	typename result_of::anon<Tag1, T>::type
    anon(T& t)
    {
        return anon<Tag1, T>( v2::_nil )( t );
    }

    template<typename Tag1,typename T>
	typename result_of::anon<Tag1, T const>::type
    anon(T const & t)
    {
        return anon<Tag1, T const>( v2::_nil )( t );
    }

}// ref
}// v2
}// assign
}// boost

#endif
