//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PIPE_CSV_RHS_HPP_ER_2010
#define BOOST_ASSIGN_V2_PUT_PIPE_CSV_RHS_HPP_ER_2010
#include <boost/mpl/apply.hpp>
#include <boost/assign/v2/ref/array/csv.hpp>
#include <boost/assign/v2/put/pipe/modulo/size_type.hpp>
#include <boost/assign/v2/put/pipe/modulo/traits.hpp>
#include <boost/assign/v2/put/pipe/csv/size_type.hpp>

namespace boost{
    struct use_default;
namespace assign{
namespace v2{
namespace put_pipe_aux{

    template<typename Pars, csv_size_type N, typename U>
    struct csv_rhs
    {
        typedef boost::use_default tag2_;
        typedef modulo_traits<Pars> modulo_traits_;
        typedef typename modulo_traits_::size pars_size;
        typedef typename modulo_traits_::cont_ pars_cont_type;
        typedef typename v2::ref::nth_result_of::csv_array<
            N,
            U
        >::type seq_arg_cont_type; //notice it's arg, not args

        csv_rhs(){}
        csv_rhs(pars_cont_type const& p, seq_arg_cont_type const& s)
            : pars_cont( p ), seq_arg( s ){}

        pars_cont_type const& pars()const
        {
            return this->pars_cont;
        }

        seq_arg_cont_type const& seq_args()const
        {
            return this->seq_arg;
        }
        seq_arg_cont_type& seq_args()
        {
            return this->seq_arg;
        }

        protected:
        pars_cont_type pars_cont;
        seq_arg_cont_type seq_arg;

    };

}// put_pipe_aux
}// v2
}// assign
}// boost

#endif
