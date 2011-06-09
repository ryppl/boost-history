//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_INTERPRETER_CSV_BASIC_ER_2011_HPP
#define BOOST_ASSIGN_V2_INTERPRETER_CSV_BASIC_ER_2011_HPP
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <boost/assign/v2/interpreter/csv/csv_ext.hpp>
#include <boost/assign/v2/support/pp/ignore.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_interpreter_csv_basic
namespace interpreter_aux{

    template<
        typename D, typename C, typename F, typename MTag, typename DTag,
        typename... Args
    >
    D const&
    csv(
        interpreter_crtp<D, C, F, MTag, DTag> const& interpreter,
        Args&&... args
    )/*<-*/
    {
        return csv<1>( interpreter, std::forward<Args>( args )... );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

}// interpreter_aux

using interpreter_aux::csv;

//]
}// v2
}// assign
}// boost

#else
#include <boost/assign/v2/interpreter/cpp03/csv_basic.hpp>
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
#endif // BOOST_ASSIGN_V2_INTERPRETER_CSV_BASIC_ER_2011_HPP
