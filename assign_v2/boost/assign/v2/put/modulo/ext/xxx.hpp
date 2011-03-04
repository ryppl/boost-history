//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODULO_EXT_XXX
#include <boost/preprocessor/cat.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/utility/enable_if.hpp>

// Needed by dependants
#include <boost/assign/v2/detail/keyword/nil.hpp>
#include <boost/assign/v2/put/modulo/modifier.hpp>

#include <boost/assign/v2/detail/pp/forward.hpp>
#include <boost/assign/v2/put/frame/crtp.hpp>
#include <boost/assign/v2/put/frame/modifier.hpp>
#include <boost/assign/v2/put/modulo/ext.hpp>

#define BOOST_ASSIGN_V2_PUT_MODULO_EXT_XXX(Param)\
namespace result_of_modulo{\
    template<typename D, typename Arg>\
    struct Param\
        : ::boost::mpl::apply1<\
            result_of_modulo::ext<D>\
            , put_parameter::Param<Arg>\
        >\
    {};\
}\
namespace put_aux{\
\
    template<typename C, typename F, typename Tag, typename D, typename Arg>\
    typename result_of_modulo::Param<D, Arg>::type\
    operator%(\
        crtp<C, F, Tag, D> const& lhs,\
        put_aux::Param<Arg> const& rhs\
    )\
    {\
        typedef result_of_modulo::Param<D, Arg> caller_;\
        return caller_::call( lhs.derived(), rhs );\
    }\
\
}\
namespace{\
    put_aux::BOOST_PP_CAT(Param,_keyword) const BOOST_PP_CAT(_,Param)\
        = put_aux::BOOST_PP_CAT(Param,_keyword)();\
}\
/**/

#endif
