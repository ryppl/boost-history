//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_AUX_LIST_HOLDER_HEAD_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_AUX_LIST_HOLDER_HEAD_ER_2010_HPP
#include <boost/assign/v2/ref/aux_/list/fwd.hpp>
#include <boost/config.hpp>
#include <boost/type_traits/add_reference.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace list_aux{

    template<typename T> struct head_holder{};

    template<typename T> struct head_holder<T&>{

        typedef T head_value_type;

        explicit head_holder(T& t) : head_( &t ){}
        
        typedef T& result_of_head_type;
        result_of_head_type head()const{ return (*this->head_); }

        private:
        T* head_;

    };

    template<>
    struct head_holder<void_>
    {
        typedef void_ head_value_type;
        head_holder(){}
    };

    template<typename T>
    struct head_value{ typedef typename T::head_value_type type; };

    template<typename T>
    struct head_reference
        : boost::add_reference<typename head_value<T>::type>
    {};

}// list_aux
}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_AUX_LIST_HOLDER_HEAD_ER_2010_HPP
