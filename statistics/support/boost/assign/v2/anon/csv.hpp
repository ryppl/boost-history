//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_ANON_CSV_ER_2010_HPP
#define BOOST_ASSIGN_V2_ANON_CSV_ER_2010_HPP
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>

#include <boost/assign/v2/detail/keyword/nil.hpp>
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>

#include <boost/assign/v2/anon/make.hpp>
#include <boost/assign/v2/anon/cont.hpp>

namespace boost{
namespace assign{
namespace v2{

#define MACRO1(z, i, data) ( BOOST_PP_CAT(_, i) )
#define MACRO2(z, N, data)\
	template<typename T>\
    typename result_of::anon<T>::type\
    csv_anon( BOOST_PP_ENUM_PARAMS(N, T const & _) )\
    {\
        return anon<T>( v2::_nil) BOOST_PP_REPEAT(N, MACRO1, ~ );\
    }\
/**/
BOOST_PP_REPEAT_FROM_TO(
	1, 
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    MACRO2,
    ~
)
#undef MACRO1
#undef MACRO2

}// v2
}// assign
}// boost

#endif
