//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_CSV_PUT_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_CSV_PUT_ER_2010_HPP
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#include <boost/assign/v2/interpreter/csv.hpp>
#include <boost/assign/v2/put/put.hpp>
#include <boost/assign/v2/option/data_generator.hpp>
#include <boost/assign/v2/option/list.hpp>
#include <boost/config.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/utility/enable_if.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#else
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
#include <boost/preprocessor/control.hpp>
#include <boost/preprocessor/repetition.hpp>
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

namespace boost{
namespace assign{
namespace v2{
//[syntax_csv_put
namespace interpreter_aux{

	template<typename F>
    struct csv_ready
    	: ::boost::mpl::true_
    {};

	template<typename T, typename K, typename M>
    struct csv_ready<
		functor_aux::pair<T, K, M>
    > 
    	: ::boost::mpl::false_
    {};

namespace result_of{

	template<typename C, typename F, typename MTag, typename DTag>
	struct if_csv_ready : ::boost::mpl::identity<
    	put_interpreter<C, F, MTag, DTag>
    >
    {};

	template<typename C, typename F, typename MTag, typename DTag>
	struct else_csv_ready : result_of::option_data_generator<
    	put_interpreter<C, F, MTag, DTag>, C, value_
    >
    {};

	template<typename C, typename F, typename MTag, typename DTag>
	struct make_csv_ready : ::boost::mpl::eval_if<
		csv_ready<F>,
        if_csv_ready<C, F, MTag, DTag>,
        else_csv_ready<C, F, MTag, DTag>
    >{};

}// result_of

	template<typename C, typename F, typename MTag, typename DTag>
    typename result_of::if_csv_ready<C, F, MTag, DTag>::type
	make_csv_ready( 
    	put_interpreter<C, F, MTag, DTag> const& interpreter, 
        boost::mpl::true_ suitable
    )
    {
		return interpreter;
	}

	template<typename C, typename F, typename MTag, typename DTag>
    typename result_of::else_csv_ready<C, F, MTag, DTag>::type
	make_csv_ready( 
    	put_interpreter<C, F, MTag, DTag> const& interpreter, 
        boost::mpl::false_ suitable
    )
    {
		return interpreter % ( _data = _value );
	}

	template<typename C, typename F, typename MTag, typename DTag>
    typename result_of::make_csv_ready<C, F, MTag, DTag>::type
	make_csv_ready( put_interpreter<C, F, MTag, DTag> const& interpreter)
    {
		return make_csv_ready( 
        	interpreter, 
            typename csv_ready<F>::type() 
        );
    }

//<-
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//->


	template<typename C, typename O, bool is, typename Args>
    typename boost::enable_if< 
    	is_option_crtp<O> 
    >::type csv_put( C& cont, option_crtp<O, is> const& crtp, Args&&... args )
    {
    	O const& options = static_cast<O const&>( crtp );
    	csv(
    		make_csv_ready( 
                put( cont ) % options
        	),
          	std::forward<Args>( args )...  
        );
    }

	template<typename C, typename Args>
    typename boost::disable_if< 
    	is_option_crtp<O> 
    >::type csv_put( C& cont, Args&&... args)
    {
    	csv(
    		make_csv_ready( put( cont ) ),
          	std::forward<Args>( args )...  
        );
    }

//]
#else

	template<typename C>
    void csv_put( C& cont ){}

#define BOOST_ASSIGN_V2_MACRO(z, N, is_const)\
    template<typename C, typename O, BOOST_PP_ENUM_PARAMS(N, typename T)>\
	typename boost::enable_if<\
    	is_option_crtp<O>\
    >::type csv_put(\
    	C& cont, O const& options,\
        BOOST_PP_ENUM_BINARY_PARAMS(N, T, BOOST_PP_EXPR_IF(is_const, const)& _)\
    )\
    {\
        csv(\
        	make_csv_ready( put( cont ) % options ) \
        	, BOOST_PP_ENUM_PARAMS(N, _)\
        );\
    }\
    template<typename C, BOOST_PP_ENUM_PARAMS(N, typename T)>\
    typename boost::disable_if<\
    	is_option_crtp<T0>\
    >::type csv_put(\
    	C& cont,\
        BOOST_PP_ENUM_BINARY_PARAMS(N, T, BOOST_PP_EXPR_IF(is_const, const)& _)\
    )\
    {\
        csv( make_csv_ready( put( cont ) ), BOOST_PP_ENUM_PARAMS(N, _) );\
    }\
/**/
BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_MACRO,
    0
)
BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_MACRO,
    1
)
#undef BOOST_ASSIGN_V2_MACRO
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

}// interpreter_aux
using interpreter_aux::csv_put;
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_CSV_PUT_ER_2010_HPP
