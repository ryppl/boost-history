//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_EXT_XXX_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_EXT_XXX_ER_2010_HPP
#include <boost/preprocessor/cat.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>

// Needed by dependants
#include <boost/assign/v2/detail/keyword/nil.hpp>
#include <boost/assign/v2/put/modulo/modifier.hpp>
#include <boost/assign/v2/put/frame/base.hpp>

#include <boost/assign/v2/detail/pp/forward.hpp>
#include <boost/assign/v2/put/frame/modifier.hpp>
#include <boost/assign/v2/put/modulo/ext.hpp>

#define BOOST_ASSIGN_V2_PUT_EXT_XXX(Param)\
namespace result_of_modulo{\
    template<typename T, typename Arg>\
    struct Param\
        : ::boost::mpl::apply1<\
            result_of_modulo::ext<T>\
            , put_parameter::Param<Arg>\
        >\
    {};\
}\
namespace put_aux{\
\
    template<typename T,typename Arg>\
    typename boost::lazy_enable_if<\
        boost::is_base_of<put_aux::put_base, T>,\
        result_of_modulo::Param<T, Arg>\
    >::type\
    operator%(\
        T const& t,\
        put_aux::Param<Arg> const& p\
    )\
    {\
        typedef result_of_modulo::Param<T, Arg> caller_;\
        return caller_::call( t, p );\
    }\
\
}\
namespace{\
    put_aux::BOOST_PP_CAT(Param,_keyword) const BOOST_PP_CAT(_,Param)\
        = put_aux::BOOST_PP_CAT(Param,_keyword)();\
}\
/**/

#endif
