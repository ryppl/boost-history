//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DEQUE_CSV_DEQUE_EXT_ER_2011_HPP
#define BOOST_ASSIGN_V2_DEQUE_CSV_DEQUE_EXT_ER_2011_HPP
#include <boost/assign/v2/deque/csv_deque/result_of.hpp>
#include <boost/assign/v2/interpreter/csv/csv_ext.hpp>
#include <boost/assign/v2/option/list.hpp>
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#include <boost/assign/v2/support/pp/ignore.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>

namespace boost{
namespace assign{
namespace v2{
//[syntax_csv_deque_ext
namespace interpreter_aux{

    template<typename T, int I, typename...Types, typename... Args>
    typename result_of::csv_deque<T>::type
    csv_deque(Args&& ...  args)/*<-*/
    {
        typename result_of::csv_deque<T>::type result;
        return csv<I, Types...>( result, std::forward<Args>( args )...);
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<typename T,
        typename Options, int I, typename...Types, typename... Args>
    typename result_of::csv_deque_option<T, Options>::type
    csv_deque(Args&& ...  args)/*<-*/
    {
        typename result_of::csv_deque_option<T, Options>::type result;
        return csv<I, Types...>( result, std::forward<Args>( args )... );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

}// interpreter_aux

    using interpreter_aux::csv_deque;
//]
}// v2
}// assign
}// boost

#else
#include <boost/assign/v2/deque/cpp03/csv_deque_ext.hpp>
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

#endif // BOOST_ASSIGN_V2_DEQUE_CSV_DEQUE_EXT_ER_2011_HPP
