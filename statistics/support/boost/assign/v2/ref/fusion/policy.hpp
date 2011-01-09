//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_FUSION_POLICY_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_FUSION_POLICY_ER_2010_HPP
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/always.hpp>
#include <boost/assign/v2/ref/array/alloc/lazy.hpp>
#include <boost/assign/v2/ref/fusion/fwd.hpp>
#include <boost/assign/v2/ref/fusion/most_const.hpp>

namespace boost{
	struct use_default;
namespace assign{
namespace v2{
namespace ref{
namespace fusion_aux{

	template<typename Tag2>
    struct policy{};

	template<>
    struct policy<boost::use_default> : boost::mpl::always<
    	boost::mpl::empty_base
    >{};

    template<>
    struct policy<alloc_tag::lazy_alloc>
    {
    	template<typename N,typename L,typename Tag1,typename T>
    	struct apply
        {
        	typedef alloc_tag::lazy_alloc tag2_;
            typedef typename fusion_aux::most_const<L, T>::type most_c_;
    		typedef fusion_aux::container<
            	N::value, 
                L, 
                Tag1, 
                tag2_, 
                T
            > derived_;
        	typedef array_aux::lazy_alloc<
            	N::value,
                Tag1, 
                most_c_, 
                derived_
            > type;
        };

	}; 

}// fusion_aux
}// ref
}// v2
}// assign
}// boost

#endif
