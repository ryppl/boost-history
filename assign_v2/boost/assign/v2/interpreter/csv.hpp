//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_INTERPRETER_CSV_ER_2010_HPP
#define BOOST_ASSIGN_V2_INTERPRETER_CSV_ER_2010_HPP
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/interpreter/fwd.hpp>
#include <boost/config.hpp>
#include <boost/mpl/bool.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#else
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/repetition.hpp>
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

namespace boost{
namespace assign{
namespace v2{
//[syntax_interpreter_csv
namespace interpreter_aux{

//<-
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//->

    template<typename C, typename F, typename MTag, typename DTag, typename D>
    void csv( 
        interpreter_crtp<C, F, MTag, DTag, D> const& interpreter
    )/*<-*/
    {
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
    template<typename C, typename F, typename MTag, typename DTag, typename D>
    void csv( 
        interpreter_crtp<C, F, MTag, DTag, D> const& interpreter, 
        T&& t, Args&&... args 
    )/*<-*/
    {
        return csv( 
            interpreter( std::forward<T>( t ) ), 
            std::forward<Args>( args )... 
        );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

//]
#else
#define BOOST_ASSIGN_V2_MACRO1(z, i, data) ( BOOST_PP_CAT(_, i) )
#define BOOST_ASSIGN_V2_MACRO2(z, N, is_const)\
    template<\
        typename C, typename F, typename MTag, typename DTag, typename D\
        BOOST_PP_ENUM_TRAILING_PARAMS(N, typename T)\
    >\
    void csv(\
        interpreter_crtp<C, F, MTag, DTag, D> const& interpreter\
        BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(\
            N, T, BOOST_PP_EXPR_IF(is_const, const) & _\
        )\
    )\
    {\
        interpreter BOOST_PP_REPEAT(N, BOOST_ASSIGN_V2_MACRO1, ~ );\
    }\
/**/

BOOST_PP_REPEAT_FROM_TO(
    1, // otherwise redefintion of csv()
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_MACRO2,
    0
)
BOOST_PP_REPEAT(
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_MACRO2,
    1
)
#undef BOOST_ASSIGN_V2_MACRO1
#undef BOOST_ASSIGN_V2_MACRO2
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

}// interpreter_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_INTERPRETER_CSV_ER_2010_HPP
