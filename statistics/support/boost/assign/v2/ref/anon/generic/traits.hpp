//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ANON_GENERIC_TRAITS_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ANON_GENERIC_TRAITS_ER_2010_HPP
#include <boost/mpl/int.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/assign/v2/ref/detail/fusion_list/fwd.hpp>
#include <boost/assign/v2/ref/detail/fusion_list/nth_result_of.hpp>
#include <boost/assign/v2/ref/anon/generic/policy.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace anon_aux{

	template<std::size_t N,typename L,typename Tag1,typename T>
    struct fusion_list
    {
    	typedef typename L::super1_t link_;
    	typedef fusion_list_aux::container<N, link_, Tag1, T> type;
    };

	template<typename L,typename T,typename Tag1>
    struct fusion_list<0, L, Tag1, T>
    {
    	typedef typename fusion_list_aux::empty_fusion_list<Tag1, T>::type type;
    };

    template<std::size_t N, typename L,typename Tag1, typename Tag2, typename T>
	struct static_array : boost::mpl::apply4<
        anon_aux::policy<Tag2>,
        boost::mpl::int_<N>, L, Tag1, T
    >{};
    
    template<std::size_t N, typename L,typename Tag1, typename Tag2,typename T>
    struct inherit
    {
        typedef typename 
        	anon_aux::fusion_list<N, L, Tag1, T>::type super1_t;
        typedef typename anon_aux::static_array<
        	N, L, Tag1, Tag2, T
        >::type super2_t;
    };

}// anon_aux
}// ref
}// v2
}// assign
}// boost

#endif
