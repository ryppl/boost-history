//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_FUSION_MAKE_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_FUSION_MAKE_ER_2010_HPP
#include <boost/mpl/vector/vector0.hpp>
#include <boost/assign/v2/detail/keyword/nil.hpp>
#include <boost/assign/v2/ref/fusion/container.hpp> 
#include <boost/assign/v2/ref/fusion/nth_result_of.hpp> 

namespace boost{
	struct use_default;
namespace assign{
namespace v2{
namespace ref{
namespace result_of{

	template<typename Tag1,typename Tag2,typename Tnil>
    struct fusion_nil : ::boost::mpl::apply1< 
        nth_result_of::fusion<Tag1, Tag2, Tnil>,
        ::boost::mpl::vector0<>
    >{};
    
}// result_of

	template<typename Tag1, typename Tag2, typename Tnil>
    typename result_of::fusion_nil<Tag1, Tag2, Tnil>::type
    fusion( keyword_aux::nil )
    {
    	typedef typename result_of::fusion_nil<
            Tag1,
            Tag2,
        	Tnil
        >::type result_;
    	return result_();
    }

	template<typename Tag1, typename Tag2>
    typename result_of::fusion_nil<
    	Tag1, 
        Tag2, 
        fusion_aux::nil_arg_type
    >::type
	fusion( keyword_aux::nil )
    {
    	typedef fusion_aux::nil_arg_type nil_arg_;
    	typedef typename result_of::fusion_nil<
            Tag1,
            Tag2,
        	nil_arg_
        >::type result_;
    	return result_();
    }
     
}// ref
}// v2
}// assign
}// boost

#endif
