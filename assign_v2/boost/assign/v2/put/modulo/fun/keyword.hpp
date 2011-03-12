//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODULO_FUN_KEYWORD_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODULO_FUN_KEYWORD_ER_2010_HPP
#include <boost/assign/v2/put/modulo/fun/make.hpp>
#include <boost/preprocessor/cat.hpp>

#define BOOST_ASSIGN_V2_MODULO_FUN_KEYWORD(NAME, FUN)\
namespace boost{\
namespace assign{\
namespace v2{\
namespace put_aux{\
\
    struct BOOST_PP_CAT(keyword_,NAME){\
\
        BOOST_PP_CAT(keyword_,NAME)(){}\
\
        template<typename T>\
        struct result{\
            typedef modulo_fun< FUN > type;\
        };\
\
        template<typename T>\
        typename result<T>::type\
        operator()()const{ return ( v2::_fun = FUN() ); }\
\
    };\
\
}\
namespace {\
    put_aux::BOOST_PP_CAT(keyword_,NAME) const BOOST_PP_CAT(_,NAME)\
        = put_aux::BOOST_PP_CAT(keyword_,NAME);\
}\
}\
}\
}\
/**/

#endif // BOOST_ASSIGN_V2_PUT_MODULO_FUN_KEYWORD_ER_2010_HPP
