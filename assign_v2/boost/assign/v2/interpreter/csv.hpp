//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_INTERPRETER_CSV_ER_2011_HPP
#define BOOST_ASSIGN_V2_INTERPRETER_CSV_ER_2011_HPP
#include <boost/assign/v2/interpreter/crtp.hpp>
#include <boost/assign/v2/interpreter/as_arg_list.hpp>
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>

namespace boost{
namespace assign{
namespace v2{
//[syntax_interpreter_csv
namespace interpreter_aux{

    template<typename D, typename C, typename F, typename MTag, typename DTag>
    void csv(
        interpreter_crtp<D, C, F, MTag, DTag> const& interpreter
    )/*<-*/
    {
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<
        typename D, typename C, typename F, typename MTag, typename DTag,
        typename R
    >
    void invoke(
        interpreter_crtp<D, C, F, MTag, DTag> const& interpreter,
        const as_arg_list_adapter<R>& adapter
    )/*<-*/
    {
        interpreter.as_arg_list( adapter );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<
        typename D, typename C, typename F, typename MTag, typename DTag,
        typename T
    >
    typename boost::disable_if<
        is_as_arg_list_adapter<
            typename boost::remove_cv<
                typename boost::remove_reference<T>::type
            >::type
        >
    >::type
    invoke(
        interpreter_crtp<D, C, F, MTag, DTag> const& interpreter,
        T&& t
    )/*<-*/
    {
        interpreter( std::forward<T>( t ) );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<typename D, typename C, typename F, typename MTag, typename DTag,
        typename T, typename... Args>
    void csv(
        interpreter_crtp<D, C, F, MTag, DTag> const& interpreter,
        T&& t, Args&&... args
    )/*<-*/
    {
        invoke( interpreter, t );
        csv(
            interpreter,
            std::forward<Args>( args )...
        );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

//]

}// interpreter_aux
}// v2
}// assign
}// boost

#else

#include <boost/assign/v2/interpreter/cpp03/csv.hpp>

#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

#endif // BOOST_ASSIGN_V2_INTERPRETER_CSV_ER_2011_HPP
