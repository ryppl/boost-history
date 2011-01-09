//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_ADAPTOR_CSV_FORWARD_ER_2010_HPP
#define BOOST_ASSIGN_V2_ADAPTOR_CSV_FORWARD_ER_2010_HPP
#include <boost/utility/enable_if.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_pipe_aux{

    template<std::size_t N, std::size_t I, typename F, typename C>
    typename boost::enable_if_c<
        N == I
    >::type
    csv_forward(
        F const& f ,
        C const& c
    )
    {}

    template<std::size_t N, std::size_t I, typename F, typename C>
    typename boost::enable_if_c<
        I < N
    >::type
    csv_forward(
        F const& f ,
        C const& c
    )
    {
        f( c[ I ] );
        csv_forward<N, I+1>(f, c);
    }

    template<typename F, typename C>
    void csv_forward(
        F const& f ,
        C const& c
    ){
        csv_forward<C::static_size, 0>(f, c );
    }

}// put_pipe_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_ADAPTOR_CSV_FORWARD_ER_2010_HPP
