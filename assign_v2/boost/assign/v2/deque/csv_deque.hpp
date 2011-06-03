//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DEQUE_CSV_DEQUE_ER_2011_HPP
#define BOOST_ASSIGN_V2_DEQUE_CSV_DEQUE_ER_2011_HPP
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/assign/v2/deque/deque.hpp>
#include <boost/assign/v2/interpreter/csv.hpp>
#include <boost/assign/v2/option/list.hpp>
#include <boost/type_traits/decay.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#endif

namespace boost{
namespace assign{
namespace v2{
//[syntax_deque_csv_deque
namespace interpreter_aux{

    template<typename T>
    struct csv_deque_deduce_value/*<-*/ : boost::decay<
        typename boost::remove_cv<
            typename boost::remove_reference<T>::type
        >::type
    >{}/*->*/;

namespace result_of{

    template<typename T>
    struct csv_deque
        : result_of::deque<
            typename csv_deque_deduce_value<T>::type
        >
    {};

    template<typename T, typename O>
    struct csv_deque_option/*<-*/
        : modulo_result<
            typename result_of::csv_deque<T>::type,
            O
        >
    {}/*->*/;

}// result_of

//<-
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//->

    // Explicit value
    template<typename T, typename... Args>
    typename result_of::csv_deque<T>::type
    csv_deque(Args&& ...  args)/*<-*/
    {
        typename result_of::csv_deque<T>::type result;
        return csv(
            result,
            std::forward<Args>( args )...
        );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<typename T, typename Options, typename... Args>
    typename result_of::csv_deque_option<T, Options>::type
    csv_deque(Args&& ...  args)/*<-*/
    {
        typename result_of::csv_deque_option<T, Options>::type result;
        return csv( result, std::forward<Args>( args )... );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    // Deduced value
    template<typename T, typename... Args>
    typename result_of::csv_deque<T>::type
    csv_deque(T&& _0, Args&& ...  args)/*<-*/
    {
        typename result_of::csv_deque<T>::type result;
        return csv(
            result,
            std::forward<T>( _0 ),
            std::forward<Args>( args )...
        );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    // Arity = I

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

//<-
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
//->

}// interpreter_aux
//<-
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//->
    using interpreter_aux::csv_deque;
//<-
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
//->

namespace result_of{

    using interpreter_aux::result_of::csv_deque;

}// result_of

//]
}// v2
}// assign
}// boost

#if !BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <boost/assign/v2/deque/cpp03/csv_deque.hpp>
#endif

#endif // BOOST_ASSIGN_V2_DEQUE_CSV_DEQUE_ER_2011_HPP
