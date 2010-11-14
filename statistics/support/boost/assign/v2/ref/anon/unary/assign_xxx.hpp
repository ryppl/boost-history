//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_ASSIGN_V2_REF_ANON_UNARY_XXX
#include <boost/assign/v2/detail/keyword/nil.hpp>
#include <boost/assign/v2/ref/anon/unary/make.hpp>
#define BOOST_ASSIGN_V2_REF_ANON_UNARY_XXX(NS, Tag1) \
 \
namespace boost{ \
namespace assign{ \
namespace v2{ \
namespace ref{ \
namespace NS{ \
namespace nth_result_of{ \
    template<std::size_t N,typename T> \
	struct anon : ref::nth_result_of::anon<N,Tag1,T>{}; \
}\
namespace result_of{ \
 \
	template<typename T> \
    struct empty_anon : ref::result_of::empty_anon< \
        Tag1, T \
    >{}; \
 \
 	template<typename T> \
    struct anon :  ref::result_of::anon< \
    	Tag1, T \
    >{}; \
 \
} \
	template<typename T> \
    typename NS::result_of::empty_anon<T>::type \
	anon( keyword_aux::nil const& ) \
    { \
    	return ref::anon<Tag1,T>( v2::_nil ); \
    } \
    template<typename T> \
	typename NS::result_of::anon<T>::type \
    anon(T& t) \
    { \
    	return ref::anon<Tag1>( t ); \
    } \
    template<typename T> \
	typename NS::result_of::anon<T const>::type \
    anon(T const& t) \
    { \
    	return ref::anon<Tag1>( t ); \
    } \
\
} \
} \
} \
} \
} \
/**/

#endif
