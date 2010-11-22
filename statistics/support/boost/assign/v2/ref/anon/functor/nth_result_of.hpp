//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ANON_UNARY_NTH_RESULT_OF2_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ANON_UNARY_NTH_RESULT_OF2_ER_2010_HPP
#include <boost/assign/v2/ref/anon/unary/tag2.hpp>
#include <boost/assign/v2/ref/fusion/nth_result_of.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace anon_aux{

    template<typename Tag1, typename T>
    struct empty_anon : fusion_aux::empty<
    	Tag1, 
        anon_aux::unary_tag2_, 
        T
    >{};

	template<std::size_t N,typename Tag1,typename T>
    struct recursive_result
    {
    	typedef typename recursive_result<
        	N-1, Tag1, T
        >::type previous_;
        typedef typename previous_::template result<T>::type type;
    };

	template<typename Tag1, typename T>
    struct recursive_result<0, Tag1, T> 
    	: empty_anon<Tag1, T>{};

}// anon_aux
namespace nth_result_of{

    template<std::size_t N,typename Tag1, typename T>
	struct anon 
    	: anon_aux::recursive_result<N, Tag1, T>
    {};

}// nth_result_of
}// ref
}// v2
}// assign
}// boost

#endif
