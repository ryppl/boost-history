//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_LIST_TUPLE_CPP0X_AS_ARGS_LIST_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_LIST_TUPLE_CPP0X_AS_ARGS_LIST_ER_2010_HPP
#include <boost/assign/v2/ref/list_tuple/list_tuple.hpp>
#include <boost/assign/v2/ref/tuple/cpp0x/as_arglist.hpp>
#include <boost/assign/v2/temporary/variadic_args_to_indices.hpp>
#include <boost/mpl/size.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace list_tuple_aux{


    // --- List  --- //

    template<
        typename F,
        list_size_type N, typename L, typename...Args
    >
    void as_args_list(
        F const& f ,
        ::boost::mpl::int_<N>,
        container<N, L, Args...> const& list
    )
    {}

    template<
        typename F,
        list_size_type I,
        list_size_type N, typename L, typename...Args
    >
    void as_args_list(
        F const& f ,
        ::boost::mpl::int_<I>,
        container<N, L, Args...> const& list
    )
    {
        {
            typedef ::boost::mpl::int_<I> int_;
            as_arglist( f, list.get( int_() ) ); // tuple
        }
        {
            typedef ::boost::mpl::int_<I+1> next_;
            as_args_list( f, next_(), list );
        }
    }

    // ------------ //

    template<typename F, typename L>
    void as_args_list(
        F const& f ,
        container<0, L> const& list
    ){}

    template<typename F,
        list_size_type N, typename L, typename...Args>
    void as_args_list(
        F const& f,
        container<N, L, Args...> const& list
    )
    {
        typedef ::boost::mpl::int_<0> int_;
        as_args_list(f, int_(), list);
    }

}// list_tuple_aux
using list_tuple_aux::as_args_list;
}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_LIST_TUPLE_CPP0X_AS_ARGS_LIST_ER_2010_HPP
