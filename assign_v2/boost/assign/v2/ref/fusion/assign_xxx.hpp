//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_FUSION_ASSIGN_XXX
#include <boost/assign/v2/detail/keyword/nil.hpp>
#include <boost/assign/v2/ref/fusion/make.hpp>
#include <boost/assign/v2/ref/fusion/nth_result_of.hpp>
#define BOOST_ASSIGN_V2_REF_FUSION_ASSIGN_XXX(NS, Tag1) \
 \
namespace boost{ \
namespace assign{ \
namespace v2{ \
namespace ref{ \
namespace NS{ \
namespace nth_result_of{ \
    template<typename Tag2, typename Tnil = fusion_aux::nil_arg_type> \
	struct fusion : ref::nth_result_of::fusion<Tag1, Tag2, Tnil>{}; \
} \
	template<typename Tag2> \
    typename ref::result_of::fusion_nil< \
    	Tag1, \
        Tag2, \
        fusion_aux::nil_arg_type \
    >::type \
	fusion( keyword_aux::nil const& ) \
    { \
    	return ref::fusion<Tag1, Tag2>( v2::_nil ); \
    } \
\
} \
} \
} \
} \
} \
/**/

#endif
