//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_COMPOSE_FORWARD_SEQ_ARGS_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_COMPOSE_FORWARD_SEQ_ARGS_ER_2010_HPP
#include <boost/mpl/int.hpp>
#include <boost/mpl/size.hpp>
#include <boost/assign/v2/put/compose/functor/forward_args.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_compose_aux{

	template<std::size_t I, std::size_t N, bool exit = (I == N)>
	struct forward_seq_args_caller
    {
        
		template<typename T,typename H>
    	static void call(T const& object, H const& h)
    	{
        	typedef boost::mpl::int_<I> index_;
        	forward_args(
        		object,
            	put_compose_aux::make_args( 
                	h.sequence_args().static_row( index_() ) 
                )
        	); 
            typedef forward_seq_args_caller<I+1, N> next_;
            next_::call( object, h );
        }

    };

	template<std::size_t I, std::size_t N>
	struct forward_seq_args_caller<I, N, true>
    {
		template<typename T, typename H>
    	static void call(T const& object, H const& h){}
    };

    template<typename T, typename H>
	void forward_seq_args(T const& object, H const& h)
    {
    	typedef put_compose_aux::forward_seq_args_caller<
        	0,
            H::static_sequence_args_size
        > caller_;
        caller_::call( object, h );
    }

}// put_compose_aux
}// v2
}// assign
}// boost

#endif
