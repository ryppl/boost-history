//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_FUSION_NTH_RESULT_OF_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_FUSION_NTH_RESULT_OF_ER_2010_HPP
#include <boost/mpl/void.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/assign/v2/ref/fusion/fwd.hpp>
#include <boost/assign/v2/ref/fusion/root.hpp>
#include <boost/assign/v2/ref/fusion/nil_arg.hpp>

namespace boost{
	struct use_default;
namespace assign{
namespace v2{
namespace ref{
namespace fusion_aux{

	template<typename Tag1, typename Tag2, typename Tnil>
    struct empty
    {
        typedef fusion_aux::container<
        	0,
            typename fusion_aux::root_type,
            Tag1,
            Tag2, 
            Tnil
        > type;
    };

    template<typename Tag1, typename Tag2, typename Tnil>
	struct nth_result
    {

		typedef typename
        	fusion_aux::empty<Tag1, Tag2, Tnil>::type state_;

		template<typename State, typename T>
        struct result : State::template result<T>{};

		template<typename Vec>
    	struct apply : boost::mpl::fold<
        	Vec,
            state_,
            result<boost::mpl::_1, boost::mpl::_2>
        >{};

	};

}// fusion_aux
namespace nth_result_of{

	template<
    	typename Tag1, typename Tag2, typename Tnil = fusion_aux::nil_arg_type
    >
	struct fusion
    {
        template<typename Vec>
        struct apply : boost::mpl::apply1<
        	fusion_aux::nth_result<Tag1, Tag2, Tnil>,
        	Vec
        >{};
	};

}// nth_result_of
}// ref
}// v2
}// assign
}// boost

#endif
