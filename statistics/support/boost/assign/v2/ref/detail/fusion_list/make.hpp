//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_CONTAINER_FUSION_LIST_MAKE_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONTAINER_FUSION_LIST_MAKE_ER_2010_HPP
#include <boost/mpl/void.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/assign/v2/detail/keyword/nil.hpp>
#include <boost/assign/v2/ref/detail/fusion_list/nth_result_of.hpp> 

namespace boost{
	struct use_default;
namespace assign{
namespace v2{
namespace ref{
namespace result_of{

	template<typename Tag,typename Tneg1>
    struct empty_fusion_list : boost::mpl::apply0< 
        nth_result_of::fusion_list<Tag,Tneg1> 
    >{};

    template<typename Tag,typename T0>
    struct fusion_list : boost::mpl::apply1< 
        nth_result_of::fusion_list<Tag,T0>,
        T0
    >{
    	// Tneg1 = T0 is needed for static_list and inconsequential otherwise.
    };
    
}// result_of

	template<typename Tag,typename Tneg1>
    typename result_of::empty_fusion_list<Tag,Tneg1>::type
	fusion_list( keyword_aux::nil )
    {
    	typedef typename result_of::empty_fusion_list<
            Tag,
        	Tneg1
        >::type result_;
    	return result_();
    }
 
    template<typename Tag,typename T>
	typename result_of::fusion_list<Tag,T>::type
    fusion_list(T& t)
    {
    	return fusion_list<Tag,T>( v2::_nil )( t );
    }

    template<typename Tag,typename T>
	typename result_of::fusion_list<Tag,T const>::type
    fusion_list(T const& t)
    {
    	return fusion_list<Tag,T const>( v2::_nil )( t );
    }
    
}// ref
}// v2
}// assign
}// boost

#endif
