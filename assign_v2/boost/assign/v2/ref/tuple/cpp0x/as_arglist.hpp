//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_TUPLE_CPP0X_AS_ARGLIST_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_TUPLE_CPP0X_AS_ARGLIST_ER_2010_HPP
#include <boost/assign/v2/ref/tuple/cpp0x/data.hpp>
#include <boost/assign/v2/temporary/variadic_args_to_indices.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace tuple_aux{

    // --- Implementation --- //

    template<typename F, typename T, typename I, I ...Values>
    void as_arglist_helper(
        F const& f,
        T const& t,
        ::boost::mpl::er2010::indices<I, Values...>
    )
    {
        f( ref::get<Values>( t )... );
    }

    template<typename F, typename ...Args>
    void as_arglist(
        F const& f,
        data<Args...> const& t
    )
    {
        typedef typename ::boost::mpl::er2010::args_to_indices<
            int,
            Args...
        >::type indices_;
        as_arglist_helper( f, t, indices_() );
    }

}// tuple_aux
using tuple_aux::as_arglist;
}// ref
}// v2
}// assign
}// boost

#endif
