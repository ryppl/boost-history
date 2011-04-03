//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CSV_ER_2010_HPP
#define BOOST_ASSIGN_V2_CSV_ER_2010_HPP
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/config.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector/vector0.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#include <boost/assign/v2/temporary/variadic_vector.hpp>
#else
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition.hpp>
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

namespace boost{
namespace assign{
namespace v2{
//[syntax_csv
namespace result_of{

    template<typename F>
    struct csv/*<-*/
    { typedef F type; }/*->*/;

}// nth_result_of
//<-
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//->

    template<typename F>
    typename result_of::csv<F const&,
        ::boost::mpl::vector0<>
    >::type
    csv( F const& functor)/*<-*/
    {
        return functor;
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<typename F, typename T, typename... Args>
    typename result_of::csv<F const&>::type
    >::type
    csv( F const& functor, T&& t, Args&&... args )/*<-*/
    {
        return csv( functor( t ), std::forward<Args>( args )... );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

//]
#else
#define BOOST_ASSIGN_V2_MACRO1(z, i, data) BOOST_PP_CAT(T, i) data
#define BOOST_ASSIGN_V2_MACRO2(z, i, data) ( BOOST_PP_CAT(_, i) )
#define BOOST_ASSIGN_V2_MACRO3(z, N, data)\
\
    template<typename F BOOST_PP_ENUM_TRAILING_PARAMS(N, typename T)>\
    typename result_of::csv<F const&>::type\
    csv( \
        F const& functor\
        BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N, T, & _)\
    )\
    {\
        return functor BOOST_PP_REPEAT(N, BOOST_ASSIGN_V2_MACRO2, ~ );\
    }\
\
    template<typename F BOOST_PP_ENUM_TRAILING_PARAMS(N, typename T)>\
    typename result_of::csv<F const&>::type\
    csv( \
        F const& functor\
        BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N, T, const & _)\
    )\
    {\
        return functor BOOST_PP_REPEAT(N, BOOST_ASSIGN_V2_MACRO2, ~ );\
    }\
\
/**/
BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_MACRO3,
    ~
)
#undef BOOST_ASSIGN_V2_MACRO1
#undef BOOST_ASSIGN_V2_MACRO2
#undef BOOST_ASSIGN_V2_MACRO3
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_CSV_ER_2010_HPP
