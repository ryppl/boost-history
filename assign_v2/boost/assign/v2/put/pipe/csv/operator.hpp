//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PIPE_CSV_OPERATOR_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_PIPE_CSV_OPERATOR_ER_2010_HPP
#include <boost/assign/v2/put/pipe/modulo/forward.hpp>
#include <boost/assign/v2/ref/array/as_arglist.hpp>
#include <boost/assign/v2/put/pipe/csv/rhs.hpp>
#include <boost/assign/v2/put/pipe/csv/size_type.hpp>
#include <boost/assign/v2/put/container/functor.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_pipe_aux{

    template<typename C,
        typename Pars, csv_size_type N, typename U>
    C& operator|(C& cont, put_pipe_aux::csv_rhs<Pars, N, U> const& c){

		v2::ref::as_arglist(
            forward_pars<Pars>(
                put( cont ),
                c.pars()
            ),
            c.seq_args()
        );
        return cont;

    }

}// put_pipe_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_PIPE_CSV_OPERATOR_ER_2010_HPP
