//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_GENERIC_NEW_FUN_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_GENERIC_NEW_FUN_ER_2010_HPP
#include <boost/mpl/apply.hpp>
#include <boost/mpl/void.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/assign/v2/put/generic/result_of_modulo.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_aux{

    template<typename F>
    struct new_fun
    {
    	
        new_fun(const F& f) : value(f){}
        
        F value;
    };
    
	template<typename T,typename F1>
    typename boost::mpl::apply1<result_of_modulo::new_fun<T>, F1>::type
	operator%(
    	T const& t, 
        put_aux::new_fun<F1> const& h
    )
    {
    	typedef typename boost::mpl::apply1<
    		result_of_modulo::new_fun<T>,
        	F1
    	>::type result_;
        return result_( t.get(), h.value, t.modifier );
    }

	struct kwd_fun{
    
		template<typename F>
    	put_aux::new_fun<F>
		operator=(F const& f)const
    	{
			return put_aux::new_fun<F>( f );
    	}

    }; 
    
}// keyword_aux
namespace{
	const put_aux::kwd_fun _fun = put_aux::kwd_fun();
}	
}// v2
}// assign
}// boost

#endif
