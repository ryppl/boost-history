//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

// Expand like this:
//	#include <boost/assign/v2/ref/anon/generic/make.hpp>
// 	#include <boost/assign/v2/ref/wrapper/xxx.hpp> // defines Tag1
// 	BOOST_ASSIGN_V2_REF_EXT_ARRAY_ASSIGN_XXX(NS,Tag1)
//
// Usage is the same as that in make.hpp but inside namespace NS, and without
// the need to specify Tag1.

#ifndef BOOST_ASSIGN_V2_REF_EXT_ARRAY_ASSIGN_XXX
#define BOOST_ASSIGN_V2_REF_EXT_ARRAY_ASSIGN_XXX(NS,Tag1) \
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
    struct anon_nil : ref::result_of::anon_nil< \
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
    typename NS::result_of::anon_nil<T>::type \
	anon( keyword_aux::nil ) \
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
