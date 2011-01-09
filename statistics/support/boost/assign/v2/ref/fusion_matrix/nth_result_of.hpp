//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_FUSION_MATRIX_NTH_RESULT_OF_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_FUSION_MATRIX_NTH_RESULT_OF_ER_2010_HPP
#include <boost/mpl/apply.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/aux_/na.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/unpack_args.hpp>
#include <boost/mpl/at.hpp>

#include <boost/assign/v2/ref/fusion/root.hpp>
#include <boost/assign/v2/ref/fusion_matrix/fwd.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>

#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
// do nothing
#else
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/mpl/aux_/na.hpp>
#include <boost/assign/v2/detail/config/limit_arity.hpp>
#endif

namespace boost{
	struct use_default;
namespace assign{
namespace v2{
namespace ref{
namespace fusion_matrix_aux{

	template<
    	typename Tag1 = ref::assign_tag::copy,
        typename Tag2 = boost::use_default
    >
    struct empty
    {
        typedef fusion_matrix_aux::container<
            0,
            typename fusion_aux::root_type,
            Tag1,
            Tag2
        > type;
    };

    template<typename Tag1,typename Tag2>
	struct nth_result
    {

		typedef typename fusion_matrix_aux::empty<
            Tag1,
            Tag2
        >::type state_;

		template<typename NewState>
        struct result
        {
        	template<
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
                typename...Args
#else
				BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(
            		BOOST_ASSIGN_V2_LIMIT_ARITY,
            		typename T,
            		boost::mpl::na
        		)
#endif
            >
            struct apply : NewState::template result<
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
                Args...
#else
            	BOOST_PP_ENUM_PARAMS(
                	BOOST_ASSIGN_V2_LIMIT_ARITY,
                    T
                )
#endif
            >{};

        };

        struct forward_op{

			template<typename NewState, typename Vec>
            struct apply{
            	typedef result<NewState> meta_;
            	typedef boost::mpl::unpack_args< meta_ > g_;
                typedef typename boost::mpl::apply1<
                    g_,
                    Vec
                >::type type;

            };

		};

		template<typename VecTypes>
    	struct apply : boost::mpl::fold<
            VecTypes,
            state_,
            forward_op
        >{};

	};

}// fusion_matrix_aux
namespace nth_result_of{

	template<
    	typename Tag1 = ref::assign_tag::copy,
        typename Tag2 = boost::use_default
	>
	struct fusion_matrix
    {

		typedef fusion_matrix_aux::nth_result<Tag1, Tag2> meta_;

        template<typename VecTypes>
        struct apply : boost::mpl::apply1<
        	meta_,
        	VecTypes
        >{};

	};

}// nth_result_of
}// ref
}// v2
}// assign
}// boost

#endif
