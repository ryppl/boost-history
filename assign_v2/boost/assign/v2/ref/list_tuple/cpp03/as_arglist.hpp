//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_LIST_TUPLE_CPP03_AS_ARGLIST_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_LIST_TUPLE_CPP03_AS_ARGLIST_ER_2010_HPP
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/mpl/int.hpp>
#include <boost/assign/v2/ref/tuple/cpp03/as_arglist.hpp>
#include <boost/assign/v2/ref/list_tuple/container.hpp>
#include <boost/assign/v2/detail/config/limit_arity.hpp>
#include <boost/assign/v2/ref/list_tuple/size_type.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace list_tuple_aux{

#define BOOST_ASSIGN_V2_params(T) BOOST_PP_ENUM_PARAMS(BOOST_ASSIGN_V2_LIMIT_ARITY,T)

	// --- Implementation --- //

    template<
        typename F,
        list_size_type N, typename L, BOOST_ASSIGN_V2_params(typename T)
    >
    void as_arglist(
        F const& f ,
        ::boost::mpl::int_<N>,
        container<N, L, BOOST_ASSIGN_V2_params(T)> const& list
    )
    {}

    template<
        typename F,
        list_size_type I,
        list_size_type N,
        typename L, BOOST_ASSIGN_V2_params(typename T)
    >
    void as_arglist(
        F const& f ,
        ::boost::mpl::int_<I>,
        container<N, L, BOOST_ASSIGN_V2_params(T)> const& list
    )
    {
        {
            typedef ::boost::mpl::int_<I> int_;
            as_arglist( f, list.get( int_() ) );
        }
        {
            typedef ::boost::mpl::int_<I+1> next_;
            as_arglist( f, next_(), list );
        }
    }

    // ----- User interface ----- //

    template<typename F, typename L>
    void as_arglist(
        F const& f ,
        container<0, L> const& list
    ){}

    template<typename F,
        list_size_type N, typename L, BOOST_ASSIGN_V2_params(typename T)>
    void as_arglist(
        F const& f,
        container<N, L, BOOST_ASSIGN_V2_params(T)> const& list
    )
    {
        typedef ::boost::mpl::int_<0> int_;
        as_arglist(f, int_(), list);
    }

#undef BOOST_ASSIGN_V2_params

}// list_tuple_aux
using list_tuple_aux::as_arglist;
}// ref
}// v2
}// assign
}// boost

#endif
