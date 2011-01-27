//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_STD_XXX
#include <boost/preprocessor/cat.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/assign/v2/detail/pp/forward.hpp>
#include <boost/assign/v2/put/frame/modifier.hpp>
#include <boost/assign/v2/put/modulo/modifier.hpp>

#define BOOST_ASSIGN_V2_PUT_STD_XXX(FUN)\
namespace boost{\
namespace assign{\
namespace v2{\
namespace modifier_tag{ struct FUN{}; }\
namespace put_aux{\
\
    template<>\
    class modifier<v2::modifier_tag::FUN>\
    {\
\
		typedef boost::accumulators::dont_care dont_care_;\
\
		public:\
\
        modifier(){}\
        modifier( dont_care_ ){}\
\
    	template<typename V, typename T>\
        void impl(V& v, BOOST_ASSIGN_V2_forward_param(T, t) )const{\
            v.FUN(\
                BOOST_ASSIGN_V2_forward_arg(T, t)\
            );\
        }\
\
    	template<typename V, typename T>\
        void impl(V& v, T* t)const{ v.FUN( t ); }\
\
    };\
\
}\
namespace{\
\
    put_modulo_aux::modifier<v2::modifier_tag::FUN> const\
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
