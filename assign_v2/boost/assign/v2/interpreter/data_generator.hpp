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
#include <boost/assign/v2/detail/traits/container.hpp>
#include <boost/assign/v2/detail/functor/constructor.hpp>
#include <boost/mpl/eval_if.hpp>
    
namespace boost{
namespace assign{
namespace v2{
//[syntax_interpreter_data
namespace interpreter_aux{
//<-

    template<
        typename C // Value or pointer-container
        , template<typename> class F = container_aux::value
    >
    struct deduce_data_generator/*<-*/
    {
        typedef functor_aux::constructor<
            typename F<C>::type
        > type;
    }/*->*/;

}// interpreter_aux
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_INTERPRETER_DATA_ER_2010_HPP
