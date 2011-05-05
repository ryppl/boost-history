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
#include <boost/utility/enable_if.hpp>
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

    template<typename O, typename T>
    struct csv_deque_option/*<-*/
        : modulo_result<
            typename result_of::csv_deque<T>::type,
            O
        >
    {}/*->*/;

}// result_of

    // Empty
    template<typename T>
    typename result_of::csv_deque<T>::type
    csv_deque()/*<-*/{
        return typename result_of::csv_deque<T>::type();
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

//<-
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//->
    template<typename T, typename... Args>
    typename /*<-*/boost::lazy_disable_if<
        is_list_option_cpp0x<T>,
        /*->*/result_of::csv_deque<T>/*<-*/
    >/*->*/::type
    csv_deque(T&& t, Args&& ...  args)/*<-*/
    {
        typename result_of::csv_deque<T>::type result;
        csv( result, std::forward<T>( t ), std::forward<Args>( args )... );
        return result;
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<typename T, typename O, typename... Args>
    typename /*<-*/boost::lazy_enable_if<
        is_list_option<O>,
        /*->*/result_of::csv_deque_option<O, T>/*<-*/
    >/*->*/::type
    csv_deque(O const& options, T&& t, Args&& ...  args)/*<-*/
    {
        typedef typename result_of::csv_deque<T>::type before_op_;
        typename result_of::csv_deque_option<
            O, T
        >::type result = before_op_() % options;
        csv( result, std::forward<T>( t ), std::forward<Args>( args )... );
        return result;
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
    using interpreter_aux::result_of::csv_deque_option;

}// result_of

//]
}// v2
}// assign
}// boost

#if !BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <boost/assign/v2/deque/cpp03/csv_deque.hpp>
#endif

#endif // BOOST_ASSIGN_V2_DEQUE_CSV_DEQUE_ER_2011_HPP
