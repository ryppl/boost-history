//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_CONTAINER_FUSION_LIST_NTH_RESULT_OF_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONTAINER_FUSION_LIST_NTH_RESULT_OF_ER_2010_HPP
#include <boost/mpl/aux_/na.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/assign/v2/ref/wrapper/deduce_assign_tag.hpp>
#include <boost/assign/v2/ref/detail/fusion_list/fwd.hpp> 

namespace boost{
	struct use_default;
namespace assign{
namespace v2{
namespace ref{
namespace fusion_list_aux{

	template<typename Tag,typename Tneg1>
    struct empty_fusion_list
    {
    	typedef boost::mpl::void_ link_;
        typedef fusion_list_aux::container<0,link_,Tag,Tneg1> type;
    };

    template<typename Tag,typename Tneg1>
	struct nth_result
    {

		typedef typename 
        	fusion_list_aux::empty_fusion_list<Tag,Tneg1>::type state_;

		template<typename State,typename T>
        struct result : State::template result<T>{};

		template<typename Vec>
    	struct apply : boost::mpl::fold<
        	Vec,
            state_,
            result<boost::mpl::_1,boost::mpl::_2>
        >{};

	};

}// fusion_list_aux
namespace nth_result_of{

	template<typename Tag,typename Tneg1>
	struct fusion_list
    {
        template<
        	typename T0 = boost::mpl::na,
        	typename T1 = boost::mpl::na,
        	typename T2 = boost::mpl::na,
        	typename T3 = boost::mpl::na,
        	typename T4 = boost::mpl::na
		> 
        struct apply : boost::mpl::apply1<
        	fusion_list_aux::nth_result<Tag,Tneg1>,
        	boost::mpl::vector<T0,T1,T2,T3,T4>
        >{};
	};

}// nth_result_of
}// ref
}// v2
}// assign
}// boost

#endif
