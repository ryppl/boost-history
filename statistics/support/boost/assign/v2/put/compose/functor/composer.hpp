//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_COMPOSE_COMPOSER_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_COMPOSE_COMPOSER_ER_2010_HPP
#include <boost/mpl/apply.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/mpl/vector.hpp>

#include <boost/config.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/assign/v2/ref/fusion/assign_copy.hpp>
#include <boost/assign/v2/ref/fusion_matrix/container.hpp>
#include <boost/assign/v2/ref/fusion_matrix/nth_result_of.hpp>
#include <boost/assign/v2/put/compose/tag.hpp>

#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#else
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/seq/for_each_product.hpp>
#include <boost/preprocessor/seq/first_n.hpp>
#include <boost/assign/v2/detail/functor/crtp_unary_and_up.hpp>
#include <boost/assign/v2/detail/config/limit_arity.hpp>
#include <boost/assign/v2/detail/pp/args.hpp>
#include <boost/assign/v2/detail/pp/params.hpp>
#include <boost/assign/v2/detail/pp/seq.hpp>

#endif

namespace boost{
	struct use_default;
namespace assign{
namespace v2{
namespace put_compose_aux{

	template<
    	typename Pars = boost::mpl::vector0<>,
    	typename SeqArgs = boost::mpl::vector0<>,
        bool enable_pars = (boost::mpl::size<SeqArgs>::value == 0)
    >
	class composer;

    template<typename Pars, typename SeqArgs, bool enable_pars>
    struct composer_result
    {
    	template<typename Args>
        struct apply
        {
        	typedef typename boost::mpl::push_back<
            	SeqArgs,
            	Args
        	>::type new_;
        	typedef composer<Pars, new_> type;
        };
    };


	template<typename Pars, typename SeqArgs, bool enable_pars>
	class composer
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//do nothing
#else
    	:	public functor_aux::crtp_unary_and_up<
    			composer<Pars, SeqArgs, enable_pars>,
        		composer_result<Pars, SeqArgs, enable_pars>
    		>
#endif
    {
    	typedef boost::use_default tag2_;
    	typedef v2::ref::assign_copy::nth_result_of::fusion<tag2_> meta1_;
		typedef ref::assign_tag::copy tag1_;
		typedef ref::nth_result_of::fusion_matrix<
        	tag1_,
            boost::use_default
        > meta2_;

        public:

		BOOST_STATIC_CONSTANT(
        	std::size_t,
            static_parameters_size = boost::mpl::size<Pars>::value
        );

		BOOST_STATIC_CONSTANT(
        	std::size_t,
            static_sequence_args_size = boost::mpl::size<SeqArgs>::value
        );

        typedef typename boost::mpl::apply1<meta1_, Pars>::type pars_cont_type;
        typedef typename boost::mpl::apply1<
        	meta2_,
            SeqArgs
        >::type seq_args_cont_type;

        composer(){}
        composer(
        	pars_cont_type const& p,
            seq_args_cont_type const& s
        )
        	: pars_cont( p ),
            seq_args_cont( s ){}

        template<typename T>
        struct modulo_result
        {
        	typedef typename boost::mpl::push_back<
    			Pars,
                T const
            >::type new_;
            typedef composer<new_, SeqArgs> type;
        };

        template<typename T>
        typename boost::lazy_enable_if_c<
        	enable_pars,
        	modulo_result<T>
        >::type
        operator%(T const& t)const
        {
        	typedef typename modulo_result<T>::type result_;
            return result_( this->pars_cont( t ), this->seq_args_cont );
        }

        template<typename Args>
        struct result : boost::mpl::apply1<
        	composer_result<Pars, SeqArgs, enable_pars>,
            Args
        >
        {};

        typename result<
        	boost::mpl::vector0<>
        >::type
        operator()()const
        {
        	typedef boost::mpl::vector0<> args_;
        	typedef typename result<args_>::type result_;
            return result_( this->pars_cont, this->seq_args_cont() );
        }



#if BOOST_ASSIGN_V2_ENABLE_CPP0X

/*
    template<typename U, typename ...Args>
    typename result<
        boost::mpl::vector<XXX>
    >::type
    operator()( U&& u, Args&&...args )const{
        typedef boost::mpl::vector<XXX> args_;
        typedef typename result<args_>::type result_;
        return result_(
        	this->pars_cont,
            this->seq_args_cont(
                std::forward<U>( u ),
                std::forward<Args>( args )...
            )
        );
    }
*/

#else
		typedef functor_aux::crtp_unary_and_up<
    		composer,
        	composer_result<Pars, SeqArgs, enable_pars>
    	> super_t;
		using super_t::operator();


#define MACRO1( z, n, data ) \
 ( BOOST_PP_CAT(_,n) ) \
/**/

#define MACRO2(z, N1, data) \
    template<BOOST_PP_ENUM_PARAMS(N1, typename U)> \
    typename result< \
        boost::mpl::vector<BOOST_PP_ENUM_PARAMS(N1, U)> \
    >::type \
    impl( BOOST_PP_ENUM_BINARY_PARAMS(N1, U, &_) )const{ \
        typedef boost::mpl::vector<BOOST_PP_ENUM_PARAMS(N1, U)> args_; \
        typedef typename result<args_>::type result_; \
        return result_( \
        	this->pars_cont, \
            this->seq_args_cont( BOOST_PP_ENUM_PARAMS(N1, _) ) \
        ); \
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

        pars_cont_type const& parameters()const{
        	return this->pars_cont;
        }
        seq_args_cont_type const& sequence_args()const{
        	return this->seq_args_cont;
        }

        protected:

        pars_cont_type pars_cont;
        seq_args_cont_type seq_args_cont;

    };

}// put_compose_aux
namespace adaptor{
namespace{
	put_compose_aux::composer<> const _put = put_compose_aux::composer<>();
}
}// adaptor
}// v2
}// assign
}// boost

#endif
