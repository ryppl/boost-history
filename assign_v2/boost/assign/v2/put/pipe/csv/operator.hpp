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
#include <boost/assign/v2/put/pipe/csv/rhs.hpp>
#include <boost/assign/v2/put/pipe/csv/size_type.hpp>
#include <boost/assign/v2/put/pipe/csv/forward.hpp>
#include <boost/assign/v2/put/container/functor.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_pipe_aux{

    template<typename V,
        typename Pars, csv_size_type N, typename U>
    V& operator|(V& v, put_pipe_aux::csv_rhs<Pars, N, U> const& c){

        csv_forward(
            forward_pars<Pars>(
                put( v ),
                c.pars()
            ),
            c.seq_args()
        );
        return v;

    }

}// put_pipe_aux
}// v2
}// assign
}// boost

#endif
