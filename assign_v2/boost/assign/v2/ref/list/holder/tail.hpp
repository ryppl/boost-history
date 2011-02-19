//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_LIST_HOLDER_TAIL_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_LIST_HOLDER_TAIL_ER_2010_HPP
#include <boost/config.hpp>
#include <boost/mpl/int.hpp>
#include <boost/assign/v2/ref/list/fwd.hpp>

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
        typedef ::boost::mpl::int_<T::size::value+1> size;
        tail_holder(T const& t) : tail_( t ){}
        
        typedef T const& result_of_tail_type;
        result_of_tail_type tail()const{ return this->tail_; }
        
        private:
        T const& tail_;
    };

    template<>
    struct tail_holder<nil> 
    {
        typedef nil tail_type;
        typedef ::boost::mpl::int_<0> size;
        tail_holder(){}
    };

    template<typename T>
    struct tail_of{ typedef typename T::tail_type type; };


}// list_aux
}// ref
}// v2
}// assign
}// boost

#endif
