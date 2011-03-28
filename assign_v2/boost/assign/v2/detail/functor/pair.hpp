//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_FUNCTOR_PAIR_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_FUNCTOR_PAIR_ER_2010_HPP
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#include <boost/assign/v2/detail/pp/ignore.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#else
#include <boost/assign/v2/detail/config/limit_arity.hpp>
#include <boost/assign/v2/detail/functor/crtp_unary_and_up.hpp>
#include <boost/mpl/always.hpp>
#include <boost/preprocessor/arithmetic.hpp>
#include <boost/preprocessor/repetition.hpp>
#endif

namespace boost{
namespace assign{
namespace v2{
//syntax_detail_functor_pair
namespace functor_aux{

    template<typename T, typename K, typename M>
    class pair/*<-*/
#if !BOOST_ASSIGN_V2_ENABLE_CPP0X
    : public functor_aux::crtp_unary_and_up<
        functor_aux::pair<T, K, M>,
        ::boost::mpl::always<T>
    >
#endif
    /*->*/
    {

        public:

        pair()/*<-*/{}BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

//<-

#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//->
        template<typename... Args>
        T operator()(K const& k, Args&&...args)const/*<-*/
        {
            return T( k, M( std::forward<Args>(args)... ) );
        }/*->*/BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
//<-

#else

        protected:
            typedef ::boost::mpl::always<T> meta_result_;
            typedef functor_aux::crtp_unary_and_up<pair, meta_result_> super_;

        public:

        using super_::operator();

    	T impl( K const& k )const{
        	return T( k, M() ); 
    	} 

#define BOOST_ASSIGN_V2_MACRO(z, N, data) \
    	template<BOOST_PP_ENUM_PARAMS(N, typename T)> \
    	T impl( K const& k BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N, T, &_) )const{ \
        	return T( k, M( BOOST_PP_ENUM_PARAMS(N, _) ) ); \
    	} \
/**/
BOOST_PP_REPEAT_FROM_TO(
	1,
    BOOST_ASSIGN_V2_LIMIT_ARITY,
    BOOST_ASSIGN_V2_MACRO,
    ~
)
#undef BOOST_ASSIGN_V2_MACRO

#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

//->
    };

}// functor_aux
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_DETAIL_FUNCTOR_PAIR_ER_2010_HPP
