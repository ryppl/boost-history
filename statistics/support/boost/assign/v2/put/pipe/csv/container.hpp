//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PIPE_CSV_CONTAINER_HPP_ER_2010
#define BOOST_ASSIGN_V2_PUT_PIPE_CSV_CONTAINER_HPP_ER_2010
#include <boost/mpl/apply.hpp>
#include <boost/assign/v2/ref/array/csv.hpp>
#include <boost/assign/v2/ref/fusion/assign_copy.hpp>

namespace boost{
	struct use_default;
namespace assign{
namespace v2{
namespace put_pipe_aux{

	template<typename Pars, int N, typename U>
    struct csv_container
    {
        typedef boost::use_default tag2_;
    	typedef v2::ref::assign_copy::nth_result_of::fusion<tag2_> meta1_;
        typedef typename boost::mpl::apply1<
            meta1_,
            Pars
        >::type pars_cont_type;
    	typedef typename v2::ref::nth_result_of::csv_array<
    		N,
        	U
    	>::type seq_arg_cont_type;

        csv_container(){}
        csv_container(pars_cont_type const& p, seq_arg_cont_type const& s)
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

