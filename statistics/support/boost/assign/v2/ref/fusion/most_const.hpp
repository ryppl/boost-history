//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_FUSION_MOST_CONST_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_FUSION_MOST_CONST_ER_2010_HPP
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/assign/v2/detail/traits/type/same_remove_cv.hpp>
#include <boost/assign/v2/ref/fusion/fwd.hpp>
#include <boost/assign/v2/ref/fusion/root.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace fusion_aux{

	// {U1,...,Un} is mapped to {T if Ui = T for all i, else T const}, if  
    //	remove_cv<Ui>::type != remove_cv<Uj>::type 
    // for some i,j, otherwise the mapping is undefined.
	template<typename L,typename T>
    struct most_const : boost::lazy_enable_if<
        v2::type_traits::same_remove_cv<typename L::head_value_type, T>,
    	boost::mpl::eval_if<
    		boost::is_const<T>,
        	boost::mpl::identity<T>,
        	most_const<
    			typename L::link_value_type,
    			typename L::head_value_type
        	>
    	>
    >
    {};
    
	template<typename T>
    struct most_const<
    	fusion_aux::root_type,
        T
    >{ 
    	typedef T type; 
    };

}// fusion_aux
}// ref
}// v2
}// assign
}// boost

#endif
