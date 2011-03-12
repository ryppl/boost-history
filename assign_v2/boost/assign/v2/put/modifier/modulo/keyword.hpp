//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODIFIER_MODULO_KEYWORD_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODIFIER_MODULO_KEYWORD_ER_2010_HPP
#include <boost/assign/v2/put/modifier/modulo/make.hpp>
#include <boost/preprocessor/cat.hpp>

#ifndef BOOST_ASSIGN_V2_PUT_MODIFIER_MODULO_PARAM
#define BOOST_ASSIGN_V2_PUT_MODIFIER_MODULO_PARAM(NAME)\
    put_aux::modulo_modifier<put_aux::BOOST_PP_CAT(keyword_,NAME)>\
/**/
#endif

#ifndef BOOST_ASSIGN_V2_PUT_MODIFIER_MODULO_KEYWORD
#define BOOST_ASSIGN_V2_PUT_MODIFIER_MODULO_KEYWORD(NAME)\
namespace boost{\
namespace assign{\
namespace v2{\
namespace put_aux{\
\
    struct BOOST_PP_CAT(keyword_,NAME){\
\
        BOOST_PP_CAT(keyword_,NAME)(){}\
\
    };\
\
}\
namespace {\
    BOOST_ASSIGN_V2_PUT_MODIFIER_MODULO_PARAM(NAME) const BOOST_PP_CAT(_,NAME)\
        = BOOST_ASSIGN_V2_PUT_MODIFIER_MODULO_PARAM(NAME)();\
}\
}\
}\
}\
/**/
#endif

// The default
BOOST_ASSIGN_V2_PUT_MODIFIER_MODULO_KEYWORD(modifier)    
BOOST_ASSIGN_V2_PUT_MODIFIER_MODULO_META_MODIFIER_TAG(modifier, Arg)

#endif // BOOST_ASSIGN_V2_PUT_MODIFIER_MODULO_KEYWORD_ER_2010_HPP

