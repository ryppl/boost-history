//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_INTERPRETER_DATA_ER_2010_HPP
#define BOOST_ASSIGN_V2_INTERPRETER_DATA_ER_2010_HPP
#include <boost/assign/v2/detail/functor/pair.hpp>
#include <boost/assign/v2/detail/functor/value.hpp>
#include <boost/assign/v2/interpreter/fwd.hpp>
    
namespace boost{
namespace assign{
namespace v2{
//[syntax_interpreter_data
namespace interpreter_aux{

    template<
        typename C 		// Value or pointer-container
        , typename T
        , bool is_map
    >
    struct deduce_data_generator/*<-*/
    {
        typedef v2::functor_aux::value<T> type; 
    }/*->*/;

    template<typename C, typename T>
    struct deduce_data_generator<C, T, true>/*<-*/
    {
        typedef v2::functor_aux::pair<
            T, 
            typename container_aux::key<C>::type,
            typename container_aux::mapped<C>::type
        > type; 
    }/*->*/;

}// interpreter_aux
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_INTERPRETER_DATA_ER_2010_HPP
