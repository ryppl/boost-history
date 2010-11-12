//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODIFIER_XXX
#include <boost/preprocessor/cat.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/assign/v2/put/modifier/def.hpp>
#include <boost/assign/v2/put/generic/new_modifier.hpp>

#define BOOST_ASSIGN_V2_PUT_MODIFIER_XXX(FUN)\
namespace boost{\
namespace assign{\
namespace v2{\
namespace modifier_tag{ struct FUN{}; }\
namespace put_aux{\
\
	template<>\
    struct modifier<v2::modifier_tag::FUN>\
    {\
\
    	modifier(){}\
        modifier( boost::accumulators::dont_care ){}\
\
    	template<typename V, typename T>\
       	void impl(V& v, T& t)const{ v.FUN( t ); }\
\
    	template<typename V, typename T>\
        void impl(V& v, T* t)const{ v.FUN( t ); }\
\
    };\
\
}\
namespace{\
\
	put_aux::set_modifier<v2::modifier_tag::FUN> const\
		BOOST_PP_CAT(_,FUN) = ( \
        	_modifier = v2::modifier_tag::FUN() \
        );\
\
}\
}\
}\
}\
/**/

#endif

/*

*/


