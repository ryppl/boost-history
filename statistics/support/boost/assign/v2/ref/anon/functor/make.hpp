//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_CONTAINER_ANON_FUNCTOR_MAKE_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONTAINER_ANON_FUNCTOR_MAKE_ER_2010_HPP
#include <boost/assign/v2/ref/anon/generic/make.hpp>
#include <boost/assign/v2/ref/anon/detail/lazy_alloc.hpp>
#include <boost/assign/v2/ref/anon/functor/nth_result_of.hpp>
 
// Usage
// using namespace boost::assign::v2::ref;
//	anon<assign_tag::name>( a )( b )( c );
 
namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace result_of{

	template<typename Tag1,typename T>
    struct anon_nil 
    	: nth_result_of::anon<0, Tag1, T>{};

	template<typename Tag1, typename T>
    struct anon 
    	: nth_result_of::anon<1, Tag1, T>{};

}// result_of

	template<typename Tag1, typename T>
    typename result_of::anon_nil<Tag1, T>::type
	anon( keyword_aux::nil )
    {
    	typedef anon_aux::ext_tag2_ tag2_;
    	return generic_anon<Tag1,tag2_,T>( v2::_nil );
    }

    template<typename Tag1,typename T>
	typename result_of::anon<Tag1, T>::type
    anon(T& t)
    {
    	typedef anon_aux::ext_tag2_ tag2_;
    	return generic_anon<Tag1,tag2_>( t );
    }

    template<typename Tag1,typename T>
	typename result_of::anon<Tag1, T const>::type
    anon(T const & t)
    {
    	typedef anon_aux::ext_tag2_ tag2_;
    	return generic_anon<Tag1,tag2_>( t );
    }

}// ref
}// v2
}// assign
}// boost

#endif
