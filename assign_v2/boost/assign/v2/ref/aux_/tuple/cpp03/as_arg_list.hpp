//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_AUX_TUPLE_CPP03_AS_ARG_LIST_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_AUX_TUPLE_CPP03_AS_ARG_LIST_ER_2010_HPP
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/assign/v2/detail/config/limit_arity.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{

#define BOOST_ASSIGN_V2_MACRO1(z, I, data) ::boost::get<I>( data )
#define BOOST_ASSIGN_V2_MACRO(z, N, data)\
    template<typename F BOOST_PP_ENUM_TRAILING_PARAMS(N, typename T)>\
    void as_arg_list(\
        F const& f,\
        ::boost::tuples::tuple<BOOST_PP_ENUM_PARAMS(N, T)> const& t\
    )\
    {\
        f( BOOST_PP_ENUM(N, BOOST_ASSIGN_V2_MACRO1, t) );\
    }\
/**/
BOOST_PP_REPEAT(
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_ARITY),
    BOOST_ASSIGN_V2_MACRO,
    ~
)
#undef BOOST_ASSIGN_V2_MACRO1
#undef BOOST_ASSIGN_V2_MACRO

}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_AUX_TUPLE_CPP03_AS_ARG_LIST_ER_2010_HPP
