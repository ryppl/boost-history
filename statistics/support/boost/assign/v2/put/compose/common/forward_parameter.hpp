//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_COMPOSE_FORWARD_PARAMETER_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_COMPOSE_FORWARD_PARAMETER_ER_2010_HPP
#include <boost/mpl/apply.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/size.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/assign/v2/put/generic/result_of_modulo.hpp>
    
namespace boost{
namespace assign{
namespace v2{
namespace put_compose_aux{

	template<std::size_t N,typename Pars, typename T, 
    	bool exit = (N == boost::mpl::size<Pars>::value)>
	struct result_of_forward_parameter
    {
    	typedef typename boost::mpl::at_c<Pars, N>::type at_;
        typedef result_of_modulo::generic<T> meta_;
        typedef typename boost::mpl::apply1<
        	meta_,
            at_
        >::type new_t_;
        
        typedef result_of_forward_parameter<N+1, Pars, new_t_> next_;
        
        typedef typename next_::type type;
        
        template<typename H>
        static type call(H const& pars_holder, T const& t)
        {
        	typedef boost::mpl::int_<N> int_;
        	return next_::call(
            	pars_holder,
                t % pars_holder.static_lookup( int_() )
            );
        }
        
    };

	template<std::size_t N,typename Pars, typename T>
	struct result_of_forward_parameter<N, Pars, T, true>
    {

		typedef T type;

        template<typename H>
        static type call(H const& pars_holder, T const& t)
        {
        	return t;
        }
    
    };

	template<typename Pars, typename T,typename C>
    typename result_of_forward_parameter<0, Pars, T>::type
	forward_parameter(
    	T const& object,
        C const& c
	)
    {
    	typedef result_of_forward_parameter<0, Pars, T> caller_;
        return caller_::call( c.parameters(), object );
	}

}// put_compose_aux
namespace result_of{

	template<typename T,typename Pars>
    struct forward_parameter : put_compose_aux::result_of_forward_parameter<
		0,
        Pars,
        T    
    >{};
    
}// result_of
}// v2
}// assign
}// boost

#endif
