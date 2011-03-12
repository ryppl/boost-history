//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ARRAY_AS_ARG_LIST_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ARRAY_AS_ARG_LIST_ER_2010_HPP
#include <boost/assign/v2/ref/array/interface.hpp>
#include <boost/assign/v2/ref/array/size_type.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace array_aux{

    // --- Implementation --- //

    template<size_type I, typename F, typename Impl, typename D>
    typename boost::enable_if_c<
        D::static_size == I
    >::type
    as_arg_list(
        F const& f ,
        interface<Impl, D> const& array
    )
    {}

    template<size_type I, typename F, typename Impl, typename D>
    typename boost::enable_if_c<
        I < D::static_size
    >::type
    as_arg_list(
        F const& f ,
        interface<Impl, D> const& array
    )
    {
        f( array[ I ].get() );
        as_arg_list<I + 1>( f, array );
    }

    // --- User interface --- //

    template<typename F, typename Impl, typename D>
    void as_arg_list(
        F const& f ,
        interface<Impl, D> const& array
    ){
        as_arg_list<0>( f, array );
    }

}// array_aux
using array_aux::as_arg_list;
}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_ARRAY_AS_ARG_LIST_ER_2010_HPP
