//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_VALUE_FUN_DEDUCE_ER_2010_HPP
#define BOOST_ASSIGN_V2_VALUE_FUN_DEDUCE_ER_2010_HPP
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
//[syntax_put_fun_deduce
namespace value_aux{

    template<typename C>
    struct container_value{ typedef typename C::value_type type; };

    template<typename /*<<Pointer-container>>*/PtrC>
    struct /*<<Meta-function mapping the `PtrC`'s pointer-type to a factory thereof>>*/ deduce_fun_pointer/*<-*/
    {
        typedef typename v2::ptr_container_aux::to_value_container<
            PtrC
        >::type cont_;
        typedef functor_aux::new_<typename cont_::value_type> type;
    }/*->*/;

    template<typename /*<<Value-container>>*/C>
    struct /*<<Meta-function mapping `C`'s value-type to a factory thereof>>*/ deduce_fun_value/*<-*/
    {
//        typedef typename v2::container_traits::value<C>::type value_type;
        typedef functor_aux::constructor<typename C::value_type> type;
    }/*->*/;

    template<typename /*<<Either of a value or pointer-container>>*/C>
    struct /*<<Meta-function mapping `C`s element-type to a factory thereof>>*/deduce_fun/*<-*/
        :  boost::mpl::eval_if<
            ptr_container_aux::is_ptr_container<C>,
            deduce_fun_pointer<C>,
            deduce_fun_value<C>
        >
    {}/*->*/;


}// value_aux
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_VALUE_FUN_DEDUCE_ER_2010_HPP
