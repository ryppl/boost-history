//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PIPE_FUNCTOR_CPP0X_FORWARD_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_PIPE_FUNCTOR_CPP0X_FORWARD_ER_2010_HPP
#include <boost/mpl/size.hpp>
#include <boost/assign/v2/ref/tuple/cpp0x.hpp>
#include <boost/assign/v2/ref/list_tuple.hpp>
#include <boost/assign/v2/temporary/variadic_args_to_indices.hpp>
#include <boost/assign/v2/put/pipe/functor/size_type.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_pipe_aux{

    // --- Tuple --- //

    template<typename F, typename T, typename I, I ...Values>
    void forward(
        F const& f,
        T const& t,
        ::boost::mpl::er2010::indices<I, Values...>
    )
    {
        f( get<Values>( t )... );
    }

    template<typename F, typename ...Args>
    void forward(
        F const& f,
        ref::tuple_aux::container<Args...> const& t
    )
    {
        typedef typename ::boost::mpl::er2010::args_to_indices<
            int,
            Args...
        >::type indices_;
        forward( f, t, indices_() );
    }

    // --- List  --- //

    template<
        typename F,
        put_pipe_aux::size_type N, typename L, typename...Args
    >
    void forward(
        F const& f ,
        ::boost::mpl::int_<N>,
        ref::list_tuple_aux::container<N, L, Args...> const& list
    )
    {}

    template<
        typename F,
        put_pipe_aux::size_type I, 
        put_pipe_aux::size_type N, typename L, typename...Args
    >
    void forward(
        F const& f ,
        ::boost::mpl::int_<I>,
        ref::list_tuple_aux::container<N, L, Args...> const& list
    )
    {
        {
            typedef ::boost::mpl::int_<I> int_;
            forward( f, list.get( int_() ) );
        }
        {
            typedef ::boost::mpl::int_<I+1> next_;
            forward( f, next_(), list );
        }
    }

    // ------------ //

    template<typename F, typename L>
    void forward(
        F const& f ,
        ref::list_tuple_aux::container<0, L> const& list
    ){}

    template<typename F, 
        put_pipe_aux::size_type N, typename L, typename...Args>
    void forward(
        F const& f,
        ref::list_tuple_aux::container<N, L, Args...> const& list
    )
    {
        typedef ::boost::mpl::int_<0> int_;
        forward(f, int_(), list);
    }

}// put_pipe_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_PIPE_FUNCTOR_FORWARD_ER_2010_HPP
