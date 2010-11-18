//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_FUSION_MATRIX_CONTAINER_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_FUSION_MATRIX_CONTAINER_ER_2010_HPP
#include <boost/mpl/apply.hpp>
#include <boost/mpl/unpack_args.hpp>
//#include <boost/mpl/aux_/na.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/assign/v2/ref/wrapper/framework.hpp>
#include <boost/assign/v2/ref/fusion/link_holder.hpp>
#include <boost/assign/v2/ref/fusion_matrix/fwd.hpp>

#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>

#else

#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/punctuation/comma.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/mpl/aux_/na.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/assign/v2/detail/config/limit_arity.hpp>
#include <boost/assign/v2/detail/functor/crtp_unary_and_up.hpp>

#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

namespace boost{
	struct use_default;
namespace assign{
namespace v2{
namespace ref{
namespace fusion_matrix_aux{

#if BOOST_ASSIGN_V2_ENABLE_CPP0X
    template<
    	std::size_t N, typename L, typename Tag1, typename Tag2,
    	typename...Args
    >
    struct meta_result{

        typedef fusion_matrix_aux::container<N, L, Tag1, Tag2,
            Args...> this_;

		template<typename ... NewArgs>
        struct apply
        {

            typedef fusion_matrix_aux::container<
            	N + 1,
                this_,
                Tag1,
                Tag2,
                typename wrapper_param<NewArgs>::type...
            > type;

        };

	};
#else
    template<
    	std::size_t N, typename L, typename Tag1, typename Tag2,
        BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(
            BOOST_ASSIGN_V2_LIMIT_ARITY,
            typename T,
            boost::mpl::na
        )
    >
    struct meta_result{

        typedef fusion_matrix_aux::container<N, L, Tag1, Tag2,
	        BOOST_PP_ENUM_PARAMS(BOOST_ASSIGN_V2_LIMIT_ARITY, T)
        > this_;

		template<
        	BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(
        		BOOST_ASSIGN_V2_LIMIT_ARITY,
            	typename U,
            	boost::mpl::na
        	)
        >
        struct apply
        {

            typedef fusion_matrix_aux::container<
            	N + 1,
                this_,
                Tag1,
                Tag2,
                BOOST_PP_ENUM_PARAMS(
                    BOOST_ASSIGN_V2_LIMIT_ARITY,
                    U
                )
            > type;

        };

	};

#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

    template<
    	std::size_t N, typename L, typename Tag1, typename Tag2,
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
    typename... Args
#else
        BOOST_PP_ENUM_PARAMS(
        	BOOST_ASSIGN_V2_LIMIT_ARITY,
            typename T
        )
#endif
    >
    class container
    	: public fusion_aux::link_holder<L, N == 0>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
// do nothing
#else
    	, public functor_aux::crtp_unary_and_up<
        	fusion_matrix_aux::container<N, L, Tag1, Tag2,
            	BOOST_PP_ENUM_PARAMS(
                	BOOST_ASSIGN_V2_LIMIT_ARITY,
                    T
                )
            >
            , boost::mpl::unpack_args<
        		fusion_matrix_aux::meta_result<N, L, Tag1, Tag2,
            		BOOST_PP_ENUM_PARAMS(
                		BOOST_ASSIGN_V2_LIMIT_ARITY,
                    	T
                	)
            	>
            >
    	>
#endif
    {

    	typedef boost::mpl::int_<0> int0_;
        typedef boost::mpl::int_<1> int1_;
        typedef boost::mpl::int_<N> size_;
        typedef typename boost::mpl::minus<size_,int1_>::type index_;
        typedef Tag1 assign_tag_;

		template<typename T>
        struct wrapper{ typedef ref::wrapper<assign_tag_,T> type; };

        typedef fusion_aux::link_holder<L, N == 0> link_;

		typedef fusion_matrix_aux::meta_result<N, L, Tag1, Tag2,
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
            Args...
#else
            BOOST_PP_ENUM_PARAMS(
                BOOST_ASSIGN_V2_LIMIT_ARITY,
                T
            )
#endif
        > meta_result_;

#if BOOST_ASSIGN_V2_ENABLE_CPP0X
// do nothing
#else
        typedef boost::mpl::na na_;
        typedef boost::tuples::null_type null_;

		typedef boost::mpl::vector<
        	BOOST_PP_ENUM_PARAMS(BOOST_ASSIGN_V2_LIMIT_ARITY, T)
        > vec_;

        template<typename T>
        struct tuple_elem : boost::mpl::eval_if<
            boost::is_same<T, na_>,
            boost::mpl::identity<null_>,
            wrapper<T>
        >{};


#define MACRO(z, n, data) typename tuple_elem<BOOST_PP_CAT(T,n)>::type

        typedef boost::tuples::tuple<
            BOOST_PP_ENUM(
                BOOST_ASSIGN_V2_LIMIT_ARITY,
                MACRO,
                ~
            )
        > tuple_;
#undef MACRO

#endif

        tuple_ tuple;

        public:

		typedef std::size_t size_type;
        BOOST_STATIC_CONSTANT(size_type, static_row_size = N);
        BOOST_STATIC_CONSTANT(size_type,
        	static_column_size =
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
                sizeof...(Args)
#else
                boost::mpl::size<vec_>::value
#endif
        );

		container(){}
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
    explicit container(
    	const L& l, Args&...args
    ) : link_( l ), tuple( ref::wrap<assign_tag_>(args)... )

//          w... = args...
    {}

#else
//#define MACRO1(z, n, data)\
//    BOOST_PP_CAT(w, n) ( BOOST_PP_CAT(_, n) )\
/**/
#define MACRO1(z, n, data) ref::wrap<assign_tag_>( BOOST_PP_CAT(data, n) )
#define MACRO(z, n, data) \
    explicit container( \
    	const L& l\
        BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(n, T, & _)\
    ) : link_( l )\
    , tuple( BOOST_PP_ENUM(n, MACRO1, _) )\
    {}\
/**/

//#define MACRO2(z, n, data) \
//    explicit container( \
//    	const L& l BOOST_PP_COMMA_IF(n) \
//        BOOST_PP_ENUM_BINARY_PARAMS(n, T, & _) \
//    ) : link_( l )\
//          BOOST_PP_ENUM_TRAILING(n, MACRO1, ~)\
//    {}\
/**/
BOOST_PP_REPEAT(
	BOOST_ASSIGN_V2_LIMIT_ARITY,
    MACRO,
    ~
)
#undef MACRO1
//#undef MACRO2
#undef MACRO
#endif
        // ------ operator() ----- //
		//       adds a row	   	   //
        // ----------------------- //

        template<
        	BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(
            	BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_ARITY),
            	typename U,
            	boost::mpl::na
        	)
        >
        struct result : boost::mpl::apply<
        	meta_result_,
            BOOST_PP_ENUM_PARAMS(
            	BOOST_ASSIGN_V2_LIMIT_ARITY,
            	U
        	)
        >{};

        typename result<>::type
        operator()()const{
        	typedef typename result<>::type result_;
        	return result_( *this );
        }

#if BOOST_ASSIGN_V2_ENABLE_CPP0X

        protected:
        template<typename ... Args>
        struct cpp0x_helper
        {

            typedef typename boost::mpl::detail::variadic_vector<
                Args...
            >::type args_;

            typedef typename boost::mpl::transform<
                args_,
                wrapper_param<boost::mpl::_>
            >::type params_;

            typedef typename boost::mpl::apply<
                meta_result_,

            >::type result_;

        };


#else
		typedef functor_aux::crtp_unary_and_up<
        	container,
            boost::mpl::unpack_args<
        		meta_result_
            >
    	> super_t;
		using super_t::operator();

#define MACRO1( z, n, data ) \
 ( BOOST_PP_CAT(_,n) ) \
/**/

#define MACRO2(z, N1, data) \
    template<BOOST_PP_ENUM_PARAMS(N1, typename U)> \
    typename result<BOOST_PP_ENUM_PARAMS(N1, U)>::type \
    impl( BOOST_PP_ENUM_BINARY_PARAMS(N1, U, &_) )const{ \
        typedef typename result<BOOST_PP_ENUM_PARAMS(N1, U)>::type result_; \
        return result_( *this BOOST_PP_ENUM_TRAILING_PARAMS(N1, _) );\
    } \
/**/
BOOST_PP_REPEAT_FROM_TO(
	1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_ARITY),
    MACRO2,
    ~
)
#undef MACRO1
#undef MACRO2

#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

		// Row extraction

        template<int I>
        struct is_head : boost::mpl::bool_< I + 1 == N >{};

        template<int I>
        struct link_result_static_row
        	: L::template result_static_row<I>{};

		template<int I>
		struct result_static_row : boost::mpl::eval_if<
        	is_head<I>,
            boost::mpl::identity<
            	container const&
            >,
            link_result_static_row<I>
        >{};

        template<int I>
        typename boost::lazy_enable_if<
        	is_head<I>,
        	result_static_row<I>
        >::type
        static_row(boost::mpl::int_<I> index)const
        {
        	return (*this);
        }

        template<int I>
        typename boost::lazy_disable_if<
        	is_head<I>,
        	result_static_row<I>
        >::type
        static_row( boost::mpl::int_<I> index )const
        {
        	return static_cast<link_ const&>(
            	*this
            ).link().static_row( index );
        }

		// Column extraction

#if BOOST_ASSIGN_V2_ENABLE_CPP0X
// TODO
#else

        template<int n>
        struct static_column_result
        {
            typedef typename boost::tuples::element<
                n,
                tuple_
            >::type wrapper_;
            typedef typename wrapper_::unwrap_type type;
        };

        template<int n>
        typename static_column_result<n>::type
        static_column( boost::mpl::int_<n> )const
        {
            return boost::get<n>( this->tuple ).unwrap();
        }

// TODO remove
// Prior to 11/2010:
/*
#define MACRO(z, n, data)\
    private: \
        typename wrapper<BOOST_PP_CAT(T,n)>::type BOOST_PP_CAT(w,n); \
    public: \
    BOOST_PP_CAT(T,n)& static_column(boost::mpl::int_<n>)const\
    {\
        return this->BOOST_PP_CAT(w,n).unwrap(); \
    }\
BOOST_PP_REPEAT(
	BOOST_ASSIGN_V2_LIMIT_ARITY,
    MACRO,
    ~
)
#undef MACRO
*/
/**/

#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

    };

}// fusion_matrix_aux
}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_FUSION_MATRIX_CONTAINER_ER_2010_HPP

