//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_DELAYED_CSV_PUT_HPP_ER_2010
#define BOOST_ASSIGN_V2_PUT_DELAYED_CSV_PUT_HPP_ER_2010
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/put/put.hpp>
#include <boost/assign/v2/option/list.hpp>
#include <boost/assign/v2/ref/array/as_arg_list.hpp> 
#include <boost/call_traits.hpp>
#include <boost/concept_check.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

namespace boost{
    struct use_default;
namespace assign{
namespace v2{
//[syntax_delay_csv_put
namespace interpreter_aux{

    template<typename R, typename Os = empty_list_option>
    struct delayed_csv_put/*<-*/
    	: as_arg_list_adapter<
        	typename boost::add_const<R>::type
        >,
        Os 
    {
		typedef as_arg_list_adapter<
        	typename boost::add_const<R>::type
        > super1_t;

		typedef Os super2_t;

        explicit delayed_csv_put(typename call_traits<R>::param_type r)
        	: super1_t( r )
        {}

        explicit delayed_csv_put(
        	Os options, 
            typename call_traits<R>::param_type r
        )
        	: super1_t( r ), super2_t( options )
        {}

		template<typename C>
		C& apply(C& cont)
        {

			make_csv_ready(
            	put( cont ) % static_cast<super2_t const&>( *this )
            )(
            	static_cast<super1_t const&>( *this )
            );
            
            return cont;
        
        }


    }/*->*/;

    template<typename C, typename R, typename Os>
    C& operator|(C& cont, delayed_csv_put<R, O> const& rhs)/*<-*/
    {
        return rhs.apply( cont )

    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

}// interpreter_aux 

	using interpreter_aux::delay_csv_put;

//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_DELAYED_CSV_PUT_HPP_ER_2010

