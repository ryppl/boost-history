//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ANON_GENERIC_POLICY_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ANON_GENERIC_POLICY_ER_2010_HPP
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/always.hpp>
#include <boost/assign/v2/ref/anon/generic/fwd.hpp>
#include <boost/assign/v2/ref/anon/detail/lazy_alloc_fwd.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace anon_aux{

	template<typename Tag2>
    struct policy{};

	template<>
    struct policy<boost::mpl::empty_base> : boost::mpl::always<
    	boost::mpl::empty_base
    >{};

    template<>
    struct policy<alloc_tag::lazy_alloc>
    {
    	template<typename N,typename L,typename Tag1,typename T>
    	struct apply
        {
        	typedef alloc_tag::lazy_alloc tag2_;
            typedef anon_aux::cont<N::value, L, Tag1, tag2_, T> d_;
        	typedef anon_aux::lazy_alloc<N::value,Tag1,T, d_> type;
        };

	}; 


}// anon_aux
}// ref
}// v2
}// assign
}// boost

#endif
