//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_AUX_LIST_HOLDER_TAIL_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_AUX_LIST_HOLDER_TAIL_ER_2010_HPP
#include <boost/config.hpp>
#include <boost/mpl/int.hpp>
#include <boost/assign/v2/ref/aux_/list/fwd.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace list_aux{

    struct nil{ nil(){} };

    template<typename T>
    struct tail_holder
    {


        typedef T tail_type;
        typedef ::boost::mpl::int_<T::static_size::value + 1> static_size;
        typedef T const& result_of_tail_type;

        tail_holder(T const& t) : tail_( t ){}
        result_of_tail_type tail()const{ return this->tail_; }

        private:
        T const tail_; // A reference would cause ref to temporary
    };

    template<>
    struct tail_holder<nil>
    {
        typedef nil tail_type;
        typedef ::boost::mpl::int_<0> static_size;
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

#endif // BOOST_ASSIGN_V2_REF_AUX_LIST_HOLDER_TAIL_ER_2010_HPP
