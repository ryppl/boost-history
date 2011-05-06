//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_AUX_LIST_HOLDER_TAIL_ER_2011_HPP
#define BOOST_ASSIGN_V2_REF_AUX_LIST_HOLDER_TAIL_ER_2011_HPP
#include <boost/assign/v2/support/keyword.hpp>
#include <boost/assign/v2/ref/aux_/list/fwd.hpp>
#include <boost/config.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/call_traits.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace list_aux{

    template<typename T>
    struct tail_holder
    {
        typedef T tail_type;
        typedef ::boost::mpl::plus<
        	typename T::tail_static_size,
            ::boost::mpl::int_<1>
        > tail_static_size;
        typedef T const& result_of_tail_type;

        tail_holder(typename boost::call_traits<T>::param_type t) 
            : tail_( t )
        {}
        
        result_of_tail_type tail()const{ return this->tail_; }

        private:
        typename boost::call_traits<T>::value_type tail_; 
    };

    template<>
    struct tail_holder<nil_>
    {
        typedef nil_ tail_type;
        typedef ::boost::mpl::int_<0> tail_static_size;
        tail_holder(){}

        typedef tail_type result_of_tail_type;
    };

    template<typename T>
    struct tail_of{ typedef typename T::tail_type type; };


}// list_aux
}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_AUX_LIST_HOLDER_TAIL_ER_2011_HPP
