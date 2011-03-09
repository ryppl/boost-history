//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ARRAY_CPP03_CSV_ARRAY_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ARRAY_CPP03_CSV_ARRAY_ER_2010_HPP
#include <boost/assign/v2/detail/keyword/nil.hpp>
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
#include <boost/assign/v2/ref/array/csv/common.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{

#define BOOST_ASSIGN_V2_MACRO1(z, i, data) result.rebind(i, BOOST_PP_CAT(data, i) );
#define BOOST_ASSIGN_V2_MACRO2(N, U)\
    template<typename T>\
    typename nth_result_of::csv_array<N, U>::type\
    csv_array( BOOST_PP_ENUM_PARAMS( N, U& _) ){ \
        return csv_array_impl<U>( BOOST_PP_ENUM_PARAMS( N, _ ) );\
    }\
/**/
#define BOOST_ASSIGN_V2_MACRO3(z, N, data)\
\
    template<typename U>\
    typename nth_result_of::csv_array<N, U>::type\
    csv_array_impl( BOOST_PP_ENUM_PARAMS( N, U& _ ) )\
    {\
        typedef typename nth_result_of::csv_array<N, U>::type result_;\
        result_ result; BOOST_PP_REPEAT( N, BOOST_ASSIGN_V2_MACRO1, _ )\
        return result;\
    }\
    BOOST_ASSIGN_V2_MACRO2(N, T)\
    BOOST_ASSIGN_V2_MACRO2(N, T const)\
\
/**/


namespace array_aux{

BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_MACRO3,
    ~
)
#undef BOOST_ASSIGN_V2_MACRO1
#undef BOOST_ASSIGN_V2_MACRO2
#undef BOOST_ASSIGN_V2_MACRO3

}// array_aux

using array_aux::csv_array;

}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_ARRAY_CPP03_CSV_ARRAY_ER_2010_HPP
