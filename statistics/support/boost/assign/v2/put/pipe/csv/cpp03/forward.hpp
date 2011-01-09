//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PIPE_FUNCTOR_CPP03_FORWARD_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_PIPE_FUNCTOR_CPP03_FORWARD_ER_2010_HPP
#include <boost/preprocessor/repetition.hpp>
#include <boost/mpl/int.hpp>
#include <boost/assign/v2/ref/list_tuple.hpp>
#include <boost/tuples/tuple.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_pipe_aux{

    // --- Tuple --- //

#define MACRO1(z, I, data) ::boost::get<I>( data ).unwrap()
#define MACRO(z, N, data)\
    template<typename F, BOOST_PP_ENUM_PARAMS(N, typename T)>\
    void forward(\
        F& f,\
        ::boost::tuple<BOOST_PP_ENUM_PARAMS(N, T)> const& t\
    )\
    {\
        f( BOOST_PP_ENUM(N, MACRO1, t) );\
    }\
/**/
BOOST_PP_REPEAT(
    BOOST_PP_INC(BOOST_PP_ASSIGN_V2_LIMIT_ARITY),
    MACRO,
    ~
)

    // --- List  --- //

    template<
        typename F,
        int N, typename L, params(typename T)
    >
    void forward(
        F& f ,
        boost::mpl::int_<N>,
        ref::list_tuple_aux::container<N, L, params(T)> const& list
    )
    {}

    template<
        typename F,
        int I, int N, typename L, params(typename T)
    >
    void forward(
        F& f ,
        boost::mpl::int_<I>,
        ref::list_tuple_aux::container<N, L, params(T)> const& list
    )
    {
        {
            typedef boost::mpl::int_<I> int_;
            forward( f, list.get( int_() ) );
        }
        {
            typedef boost::mpl::int_<I+1> next_;
            forward( f, next_(), list );
        }
    }

    // ------------ //

    template<typename F, typename L>
    void forward(
        F& f ,
        ref::list_tuple_aux::container<
            0,
            L,
            params(list_tuple_aux::na_)
        > const& list
    ){}

    template<typename F, int N, typename L, params(T)>
    void forward(
        F& f,
        ref::list_tuple_aux::container<N, L, params(T)> const& list
    )
    {
        typedef boost::mpl::int_<0> int_;
        forward(f, int_(), list);
    }

}// put_pipe_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_PIPE_FUNCTOR_FORWARD_ER_2010_HPP
