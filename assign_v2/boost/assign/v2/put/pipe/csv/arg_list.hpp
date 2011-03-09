//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PIPE_CSV_ARG_LIST_HPP_ER_2010
#define BOOST_ASSIGN_V2_PUT_PIPE_CSV_ARG_LIST_HPP_ER_2010
#include <boost/assign/v2/ref/array/csv.hpp>
#include <boost/assign/v2/put/pipe/csv/size_type.hpp>
#include <boost/assign/v2/put/pipe/modulo_traits.hpp>

namespace boost{
    struct use_default;
namespace assign{
namespace v2{
namespace put_pipe_aux{

    template<typename Pars, csv_size_type N, typename U>
    struct arg_list
    {
        typedef boost::use_default tag2_;
        typedef modulo_traits<Pars> modulo_traits_;
        typedef typename modulo_traits_::size par_list_size; // TODO needed?
        typedef typename modulo_traits_::cont_ par_list_cont_type;
        typedef typename v2::ref::nth_result_of::csv_array<
            N,
            U
        >::type arg_list_cont_type; //notice it's arg, not args

        arg_list(){}
        arg_list(par_list_cont_type const& a, arg_list_cont_type const& b)
            : par_list_cont_( a ), arg_list_cont_( b ){}

        par_list_cont_type const& par_list_cont()const 
        {
            return this->par_list_cont_;
        }

        arg_list_cont_type& arg_list_cont() // TODO needed (non-const)?
        {
            return this->arg_list_cont_;
        }

        arg_list_cont_type const& arg_list_cont() const
        {
            return this->arg_list_cont_;
        }

        protected:
        par_list_cont_type par_list_cont_;
        arg_list_cont_type arg_list_cont_;

    };

}// put_pipe_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_PIPE_CSV_ARG_LIST_HPP_ER_2010
