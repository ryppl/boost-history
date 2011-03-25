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
#include <boost/assign/v2/detail/traits/ptr_container/to_value_container.hpp>
#include <boost/assign/v2/detail/traits/ptr_container/meta.hpp>
#include <boost/assign/v2/detail/functor/constructor.hpp>
#include <boost/assign/v2/detail/functor/new.hpp>
#include <boost/mpl/eval_if.hpp>

#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/is_pointer.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_interpreter_data
namespace interpreter_aux{

    template<typename C>
    struct container_value{ typedef typename C::value_type type; };

    template<typename PtrC // Pointer-container 
        ,template<typename > class F = container_value
    >
    struct deduce_ptr_generator/*<-*/
    {
        
        typedef typename v2::container_aux::to_value_container<
            PtrC
        >::type cont_;
        typedef functor_aux::new_<
            typename F<cont_>::type
        > type;
    }/*->*/;

    template<
        typename C // Value-container 
        , template<typename > class F = container_value
    >
    struct deduce_value_generator/*<-*/
    {
        typedef functor_aux::constructor<
            typename F<C>::type
        > type;
    }/*->*/;

    template<
        typename C // Value or pointer-container
    >
    struct deduce_data_generator/*<-*/
        :  boost::mpl::eval_if<
            container_aux::is_ptr_container<C>,
            deduce_ptr_generator<C>,
            deduce_value_generator<C>
        >
    {}/*->*/;

}// interpreter_aux
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_INTERPRETER_DATA_ER_2010_HPP
