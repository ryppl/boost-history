//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_AUX_LIST_TUPLE_CPP03_CONTAINER_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_AUX_LIST_TUPLE_CPP03_CONTAINER_ER_2010_HPP
#include <boost/assign/v2/detail/config/limit_arity.hpp>
#include <boost/assign/v2/detail/functor/crtp_unary_and_up.hpp>
#include <boost/call_traits.hpp>
#include <boost/config.hpp>
#include <boost/mpl/aux_/na.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/unpack_args.hpp>
#include <boost/preprocessor/arithmetic.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace list_tuple_aux{

    typedef ::boost::tuples::null_type null_type;
    typedef ::boost::mpl::na na_type;

#define params_default(U, Na)\
    BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(\
        BOOST_ASSIGN_V2_LIMIT_ARITY,\
        U,\
        Na\
    )\
/**/
#define params(U)\
    BOOST_PP_ENUM_PARAMS(\
        BOOST_ASSIGN_V2_LIMIT_ARITY,\
        U\
    )\
/**/

    template<typename T> struct tuple_value : boost::add_reference<T>{};
    template<> struct tuple_value<na_type>{ typedef null_type type; };

    template<typename T>
    struct tuple_param{
        typedef typename boost::tuples::access_traits<
            typename tuple_value<T>::type
        >::parameter_type type;
    };

    template<typename T> struct vec_value{ typedef T type; };
    template<> struct vec_value<na_type>{ typedef na_type type; };

    template<params( typename T )>
    struct head_value
    {
#define BOOST_ASSIGN_V2_MACRO(z, n, T)\
    typename tuple_value<\
        BOOST_PP_CAT(T, n)\
    >::type\
/**/
        typedef boost::tuples::tuple<
            BOOST_PP_ENUM(
                BOOST_ASSIGN_V2_LIMIT_ARITY,
                BOOST_ASSIGN_V2_MACRO,
                T
            )
        > type;
#undef BOOST_ASSIGN_V2_MACRO


    };

    template<typename NewState>
    struct result
    {
        template<params_default(typename T, na_type)>
        struct apply : NewState::template result<params(T)>{};
    };

    template<list_size_type N,
        typename Tail, params_default(typename T, na_type)>
    class container;

    template<list_size_type N, typename Tail, 
    	params_default(typename T, na_type)>
    struct traits
    {
        typedef container<N, Tail, params(T)> cont_;
        typedef typename head_value<params(T)>::type head_value_;
        typedef fetch_tuple<N, Tail, head_value_, cont_> fetch_tuple_;
        struct meta_result{
        	template<params_default(typename U, na_type)>
        	struct apply
        	{
            	typedef container<N + 1, cont_, params(U)> type;
        	};
        };
        typedef ::boost::mpl::unpack_args<meta_result> unpacked_; 
        typedef functor_aux::crtp_unary_and_up<cont_, unpacked_> crtp_;

    };

    template<list_size_type N, typename Tail, params(typename T)>
    class container :
        public traits<N, Tail, params(T)>::crtp_,
        public traits<N, Tail, params(T)>::fetch_tuple_
    {
    
        typedef traits<N, Tail, params(T)> traits_;
        typedef typename traits_::meta_result meta_result_;
        typedef typename traits_::head_value_ head_value_;
        typedef typename traits_::crtp_ crtp_;
        typedef typename ::boost::mpl::eval_if_c<
            N == 0,
            ::boost::mpl::identity< Tail >,
            ::boost::mpl::identity< Tail const& >
        >::type tail_value_;

		tail_value_ t;
        head_value_ h;

        public:
        typename boost::call_traits<tail_value_>::const_reference 
        	tail()const{ return this->t; }
        typename boost::call_traits<head_value_>::const_reference 
        	head()const{ return this->h; }

        typedef list_tuple_aux::list_size_type list_size_type;
        typedef list_tuple_aux::tuple_size_type tuple_size_type;
        BOOST_STATIC_CONSTANT( list_size_type, static_get_size = N );
        BOOST_STATIC_CONSTANT(
            tuple_size_type,
            static_tuple_size = ::boost::tuples::length<head_value_>::value
        );

        container()
        {
            BOOST_STATIC_ASSERT( N == 0 );
        }

    	explicit container(const Tail& t_)
    	: t( t )
    	{
        	BOOST_STATIC_ASSERT( N > 0 );
    	}

#define BOOST_ASSIGN_V2_MACRO1(T,R)\
    typename tuple_param<T>::type R\
/**/
#define BOOST_ASSIGN_V2_MACRO(z, n, T)\
    BOOST_ASSIGN_V2_MACRO1(\
        BOOST_PP_CAT(T,BOOST_PP_INC(n)),\
        BOOST_PP_CAT(_,BOOST_PP_INC(n))\
    )\
        = boost::tuples::detail::cnull()\
/**/
    explicit container(
        const Tail& t_,
        BOOST_ASSIGN_V2_MACRO1(T0, _0) // (*)
        BOOST_PP_ENUM_TRAILING(
            BOOST_PP_DEC(BOOST_ASSIGN_V2_LIMIT_ARITY),
            BOOST_ASSIGN_V2_MACRO,
            T
        )
    )
    : t( t_ )
    , h(
        BOOST_PP_ENUM_PARAMS(BOOST_ASSIGN_V2_LIMIT_ARITY, _)
    )
    {
        BOOST_STATIC_ASSERT( N > 0 );
    }
#undef BOOST_ASSIGN_V2_MACRO1
#undef BOOST_ASSIGN_V2_MACRO

        template<params_default(typename U, na_type)>
        struct result : meta_result_::template apply<params(U)>{};

        using crtp_::operator();

        typename result<>::type
        operator()()const{
            typedef typename result<>::type result_;
            return result_( *this );
        }

#define BOOST_ASSIGN_V2_MACRO(z, n, data)\
        template<BOOST_PP_ENUM_PARAMS(n, typename U)>\
        typename result<BOOST_PP_ENUM_PARAMS(n, U)>::type\
        impl( BOOST_PP_ENUM_BINARY_PARAMS(n, U, &_) )const\
        {\
            typedef typename result<\
                BOOST_PP_ENUM_PARAMS(n, U)\
            >::type result_type;\
            return result_type(\
                *this,\
                BOOST_PP_ENUM_PARAMS(n, _)\
            );\
        }\
/**/
BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_ARITY),
    BOOST_ASSIGN_V2_MACRO,
    ~
)

    };

#undef BOOST_ASSIGN_V2_MACRO
#undef params
#undef params_default

}// list_tuple_aux
}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_AUX_LIST_TUPLE_CPP03_CONTAINER_ER_2010_HPP
