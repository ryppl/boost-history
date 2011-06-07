//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_CSV_PUT_EXT_ER_2011_HPP
#define BOOST_ASSIGN_V2_PUT_CSV_PUT_EXT_ER_2011_HPP
#include <boost/assign/v2/put/put.hpp>
#include <boost/assign/v2/interpreter/csv/csv_ext.hpp>
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>

namespace boost{
namespace assign{
namespace v2{
//[syntax_csv_put_ext
namespace interpreter_aux{

    template<int I,  typename ...Cast, typename C, typename... Args>
    void csv_put(C& cont, Args&&... args)/*<-*/
    {
        csv<I, Cast...>(
            put( cont ),
            std::forward<Args>( args )...
        );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    // Default constructible options

    template<typename Options,
        int I,  typename ...Cast, typename C, typename... Args>
    void csv_put(C& cont, Args&&... args)/*<-*/
    {
        csv<I, Cast...>(
            put( cont ) % Options(),
            std::forward<Args>( args )...
        );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

}// interpreter_aux

using interpreter_aux::csv_put;
//]
}// v2
}// assign
}// boost

#else
#include <boost/assign/v2/put/cpp03/csv_put_ext.hpp>
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

#endif // BOOST_ASSIGN_V2_PUT_CSV_PUT_EXT_ER_2011_HPP
