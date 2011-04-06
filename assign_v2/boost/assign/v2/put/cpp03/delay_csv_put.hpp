//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_CPP03_DELAY_CSV_PUT_HPP_ER_2010
#define BOOST_ASSIGN_V2_PUT_CPP03_DELAY_CSV_PUT_HPP_ER_2010
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
#include <boost/mpl/aux_/na.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_delay_csv_put
namespace interpreter_aux{

    template<typename U>
    typename result_of::delay_csv_put<U, 0>::type
    delay_csv_put()
    {
        return typename result_of::delay_csv_put<U, 0>::type(
            ref::csv_array<U>();
        );
    }

#define BOOST_ASSIGN_V2_MACRO2(z, N, U)\
    template<typename T, arg_list_size_type N>\
    typename result_of::delay_csv_put<U, N>::type\
    delay_csv_put( BOOST_PP_ENUM_PARAMS(N, U &_) )\
    {\
        return typename result_of::delay_csv_put<U, N>::type(\
            ref::csv_array<U>( BOOST_PP_ENUM_PARAMS(N, _) )\
        );\
    }\
    template<typename T, typename O, bool is, arg_list_size_type N>\
    typename result_of::delay_csv_put<U, N>::type\
    delay_csv_put(\
        option_crtp<O, is> const& crtp,\
        BOOST_PP_ENUM_PARAMS(N, U &_)\
    )\
    {\
        typedef O const& options_;\
        options_ options = static_cast<options_>( crtp );\
        return typename result_of::delay_csv_put<U, N>::type(\
            ref::csv_array<U>( BOOST_PP_ENUM_PARAMS(N, _) )\
        );\
    }\
/**/

BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_MACRO2,
    T
)
BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_MACRO2,
    T const
)
#undef BOOST_ASSIGN_V2_MACRO1
#undef BOOST_ASSIGN_V2_MACRO2

}// interpreter_aux 

	using interpreter_aux::delay_csv_put;

}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_CPP03_DELAY_CSV_PUT_HPP_ER_2010

