//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_CSV_PUT_BASIC_ER_2011_HPP
#define BOOST_ASSIGN_V2_PUT_CSV_PUT_BASIC_ER_2011_HPP
#include <boost/assign/v2/interpreter/csv/csv_basic.hpp>
#include <boost/assign/v2/option/data.hpp>
#include <boost/assign/v2/put/put.hpp>
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <utility>
#endif

namespace boost{
namespace assign{
namespace v2{
//[syntax_csv_put_basic
namespace interpreter_aux{
//<-
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//->

    template<typename C, typename... Args>
    void csv_put(C& cont, Args&&... args)/*<-*/
    {
        csv(
            put( cont ),
            std::forward<Args>( args )...
        );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<typename Options, typename C, typename... Args>
    void csv_put(C& cont, Args&&... args)/*<-*/
    {
        csv(
            put<Options, C>( cont ),
            std::forward<Args>( args )...
        );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

//<-
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
//->

}// interpreter_aux
//<-
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//->
    using interpreter_aux::csv_put;
//<-
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
//->
//]

}// v2
}// assign
}// boost

#if !BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <boost/assign/v2/put/cpp03/csv_put_basic.hpp>
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

#endif // BOOST_ASSIGN_V2_PUT_CSV_PUT_BASIC_ER_2011_HPP
