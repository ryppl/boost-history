//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DEQUE_CSV_DEQUE_RESULT_OF_ER_2011_HPP
#define BOOST_ASSIGN_V2_DEQUE_CSV_DEQUE_RESULT_OF_ER_2011_HPP
#include <boost/assign/v2/deque/deque.hpp>
#include <boost/type_traits/decay.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_result_of_csv_deque
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
}// interpreter_aux
namespace result_of{

    using interpreter_aux::result_of::csv_deque;

}// result_of
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_DEQUE_CSV_DEQUE_RESULT_OF_ER_2011_HPP
