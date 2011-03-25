//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DEQUE_CPP03_CSV_DEQUE_ER_2010_HPP
#define BOOST_ASSIGN_V2_DEQUE_CPP03_CSV_DEQUE_ER_2010_HPP
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace interpreter_aux{

#define BOOST_ASSIGN_V2_MACRO1(z, i, data) ( BOOST_PP_CAT(_, i) )
#define BOOST_ASSIGN_V2_MACRO2(z, N, data)\
    template<typename T>\
    typename result_of::csv_deque<T>::type\
    csv_deque( BOOST_PP_ENUM_PARAMS(N, T const & _) )\
    {\
    	typedef typename result_of::csv_deque<T>::type result_;\
        return result_() BOOST_PP_REPEAT(N, BOOST_ASSIGN_V2_MACRO1, ~ );\
    }\
/**/
BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_MACRO2,
    ~
)
#undef BOOST_ASSIGN_V2_MACRO1
#undef BOOST_ASSIGN_V2_MACRO2

}// interpreter_aux

using interpreter_aux::csv_deque;

}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_DEQUE_CPP03_CSV_DEQUE_ER_2010_HPP
