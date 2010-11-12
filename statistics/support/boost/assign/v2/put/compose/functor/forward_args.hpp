//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_COMPOSE_FORWARD_ARGS_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_COMPOSE_FORWARD_ARGS_ER_2010_HPP
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#include <boost/mpl/aux_/na.hpp>
#include <boost/mpl/int.hpp>

//#include <boost/assign/v2/detail/keyword/default.hpp>
#include <boost/assign/v2/detail/config/limit_arity.hpp>
#include <boost/assign/v2/ref/fusion_matrix/container.hpp>
#include <boost/assign/v2/ref/wrapper/framework.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_compose_aux{

	template<
    	std::size_t N, typename L,typename Tag1, typename Tag2,
        BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(	
        	BOOST_ASSIGN_V2_LIMIT_ARITY, 
            typename T,
            boost::mpl::na
        )
    >
	class args : ref::wrapper<
    	ref::assign_tag::copy,
		ref::fusion_matrix_aux::container<N, L, Tag1, Tag2,
        	BOOST_PP_ENUM_PARAMS(BOOST_ASSIGN_V2_LIMIT_ARITY, T)
        > const
    >
    {
		typedef ref::fusion_matrix_aux::container<N, L, Tag1, Tag2, 
        	BOOST_PP_ENUM_PARAMS(BOOST_ASSIGN_V2_LIMIT_ARITY, T)
        > value_;
        typedef ref::assign_tag::copy assign_tag_;
		typedef ref::wrapper<assign_tag_, value_ const> super_t;
        
        public:
        
        args(){};
        
        args(value_ const& v) : super_t( v ){}

		value_ const& value()const{ return this->unwrap(); }
    
    };

	template<
    	std::size_t N, typename L,typename Tag1, typename Tag2,
        BOOST_PP_ENUM_PARAMS(BOOST_ASSIGN_V2_LIMIT_ARITY, typename T)
    >
	args<N, L, Tag1, Tag2, 
    	BOOST_PP_ENUM_PARAMS(BOOST_ASSIGN_V2_LIMIT_ARITY, T)
    >
    make_args(
    	ref::fusion_matrix_aux::container<N,L,Tag1, Tag2, 
    		BOOST_PP_ENUM_PARAMS(BOOST_ASSIGN_V2_LIMIT_ARITY, T)
    	> const& v
    ){
    	typedef args<N,L,Tag1, Tag2, 
    		BOOST_PP_ENUM_PARAMS(BOOST_ASSIGN_V2_LIMIT_ARITY, T)
    	> result_;
        return result_( v );
    }

#define MACRO1(z, n, data) \
	BOOST_PP_COMMA_IF(n)\
	input.value().static_column(\
    	boost::mpl::int_<n>()\
    )\
/**/

#define MACRO2(z, K, data)\
	template<\
    	typename O,\
    	std::size_t N, typename L,typename Tag1, typename Tag2\
        BOOST_PP_ENUM_TRAILING_PARAMS(K, typename T)\
    >\
    O const&\
	forward_args(\
    	O const& object,\
        put_compose_aux::args<N, L, Tag1, Tag2 \
        	BOOST_PP_ENUM_TRAILING_PARAMS(K, T)\
        > const& input\
	)\
    {\
    	return object(\
            BOOST_PP_REPEAT(K, MACRO1, ~)\
        );\
	}\
/**/

    
BOOST_PP_REPEAT(
	BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_ARITY),
    MACRO2,
    ~
)

#undef MACRO1
#undef MACRO2

}// put_compose_aux
}// v2
}// assign
}// boost

#endif
