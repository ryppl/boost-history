//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PIPE_FUNCTOR_OPERATOR_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_PIPE_FUNCTOR_OPERATOR_ER_2010_HPP
#include <boost/assign/v2/put/pipe/modulo/forward.hpp>
#include <boost/assign/v2/put/pipe/functor/rhs.hpp>
#include <boost/assign/v2/put/pipe/functor/forward.hpp>
#include <boost/assign/v2/put/container/functor.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_pipe_aux{

    template<
        typename V,
        typename Pars,
        typename SeqArgs,
        bool enable_pars
    >
    V& operator|(
        V& v,
        put_pipe_aux::rhs<Pars, SeqArgs, enable_pars> const& c
    ){
        forward(
            forward_pars<Pars>(
                put( v ),
                c.pars()
            ),
            c.seq_args()
        );
        return v;
    }

}// put_pipe_aux
namespace{

    put_pipe_aux::rhs<> const _put = put_pipe_aux::rhs<>();

}
}// v2
}// assign
}// boost

#endif
