//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ANON_GENERIC_NTH_RESULT_OF_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ANON_GENERIC_NTH_RESULT_OF_ER_2010_HPP
#include <boost/mpl/void.hpp>
#include <boost/assign/v2/ref/anon/generic/cont.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace anon_aux{

    template<typename Tag1, typename Tag2,typename T>
    struct empty_generic_anon
    {
    	typedef boost::mpl::void_ void_;
    	typedef anon_aux::cont<0,void_,Tag1,Tag2,T> type;
    };

	template<std::size_t n,typename Tag1,typename Tag2,typename T>
    struct recursive_result
    {
    	typedef typename recursive_result<
        	n-1, Tag1, Tag2, T
        >::type previous_;
        typedef typename previous_::result_type type;
    };

	template<typename Tag1,typename Tag2, typename T>
    struct recursive_result<0, Tag1, Tag2, T> 
    	: empty_generic_anon<Tag1, Tag2, T>{};

}// fusion_list_aux
namespace nth_result_of{

    template<std::size_t N,typename Tag1,typename Tag2,typename T>
	struct generic_anon 
    	: anon_aux::recursive_result<N,Tag1,Tag2,T>
    {};

}// nth_result_of
}// ref
}// v2
}// assign
}// boost

#endif
