//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PIPE_FUNCTOR_RHS_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_PIPE_FUNCTOR_RHS_ER_2010_HPP
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/apply.hpp>

#include <boost/assign/v2/put/pipe/modulo/size_type.hpp>
#include <boost/assign/v2/put/pipe/modulo/traits.hpp>

#include <boost/assign/v2/ref/list_tuple.hpp>

#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#include <boost/assign/v2/temporary/variadic_vector.hpp>
#else
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/assign/v2/detail/functor/crtp_unary_and_up.hpp>
#endif

namespace boost{
    struct use_default;
namespace assign{
namespace v2{
namespace put_pipe_aux{

    template<
        typename Pars = ::boost::mpl::vector0<>,
        typename SeqArgs = ::boost::mpl::vector0<>,
        bool enable_pars = (boost::mpl::size<SeqArgs>::value == 0)
    >
    class rhs;

    template<typename Pars, typename SeqArgs, bool enable_pars>
    struct rhs_result_helper
    {

        template<typename V>
        struct apply
        {
            typedef typename ::boost::mpl::push_back<
                SeqArgs,
                V
            >::type new_;
            typedef put_pipe_aux::rhs<
                Pars,
                new_
            > type;
        };

    };

#if! BOOST_ASSIGN_V2_ENABLE_CPP0X
    typedef ref::list_tuple_aux::na_type na_type;
#endif

    template<typename Pars, typename SeqArgs, bool enable_pars>
    struct rhs_result
    {
        typedef rhs_result_helper<
            Pars,
            SeqArgs,
            enable_pars
        > helper_;

#if BOOST_ASSIGN_V2_ENABLE_CPP0X

        template<typename...Args>
        struct apply : helper_::template apply<
            typename ::boost::mpl::detail::variadic_vector<
                Args...
            >::type
        >
        {};

#else

        template<typename V>
        struct apply : helper_::template apply<V>{};

#endif

    };


    template<typename Pars, typename SeqArgs, bool enable_pars>
    class rhs
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//do nothing
#else
        :    public functor_aux::crtp_unary_and_up<
                rhs<Pars, SeqArgs, enable_pars>,
                rhs_result<Pars, SeqArgs, enable_pars>
            >
#endif
    {
        typedef boost::use_default list_tag_;
        typedef modulo_traits<Pars> modulo_traits_;
        typedef ref::nth_result_of::list_tuple meta2_;

        public:

        typedef typename modulo_traits_::size pars_size;
        typedef typename modulo_traits_::cont_ pars_cont_type;

        BOOST_STATIC_CONSTANT(
            std::size_t,
            seq_args_size = ::boost::mpl::size<SeqArgs>::value
        );

        typedef typename ::boost::mpl::apply1<
            meta2_,
            SeqArgs
        >::type seq_args_cont_type;

        rhs(){}
        explicit rhs(
            pars_cont_type const& p,
            seq_args_cont_type const& s
        ) : pars_cont( p ),
        seq_args_cont( s ){}

        // operator%

        template<typename T>
        struct modulo_result
        {
            typedef typename modulo_traits_::
                template new_pars<T>::type new_;
            typedef rhs<new_, SeqArgs> type;
        };

        template<typename T>
        typename boost::lazy_enable_if_c<
            enable_pars,
            modulo_result<T>
        >::type
        operator%(T const& t)const
        {
            typedef typename modulo_result<T>::type result_;
            return result_(
                this->pars_cont( t ),
                this->seq_args_cont
            );
        }

        // operator()

        template<
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
            typename...Args
#else
            typename VArgs
#endif
        >
        struct result : rhs_result<
            Pars,
            SeqArgs,
            enable_pars
        >::template apply<
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
            Args...
#else
            VArgs
#endif
        >{};

#if BOOST_ASSIGN_V2_ENABLE_CPP0X

        public:

        template<typename ...Args>
        typename result<Args...>::type
        operator()(Args&&...args )const
        {
            typedef typename result<Args...>::type result_;
            return result_(
                this->pars_cont,
                this->seq_args_cont(
                    std::forward<Args>( args )...
                )
            );
        }

#else
        protected:

        typedef functor_aux::crtp_unary_and_up<
            rhs,
            rhs_result<Pars, SeqArgs, enable_pars>
        > super_t;

        typedef ::boost::mpl::vector0<> v0_;

        public:

        typename result<v0_>::type
        operator()()const
        {
            typedef typename result<v0_>::type result_;
            return result_(
                this->pars_cont,
                this->seq_args_cont()
            );
        }

        using super_t::operator();

#define BOOST_ASSIGN_V2_MACRO1( z, n, data )\
 ( BOOST_PP_CAT(_,n) )\
/**/
#define BOOST_ASSIGN_V2_MACRO2(z, N1, data)\
    template<BOOST_PP_ENUM_PARAMS(N1, typename U)>\
    typename result<\
        ::boost::mpl::vector<\
            BOOST_PP_ENUM_PARAMS(N1, U)\
        >\
    >::type\
    impl( BOOST_PP_ENUM_BINARY_PARAMS(N1, U, &_) )const{\
        typedef ::boost::mpl::vector<\
            BOOST_PP_ENUM_PARAMS(N1, U)\
        > v_;\
        typedef typename result<v_>::type result_;\
        return result_(\
            this->pars_cont,\
            this->seq_args_cont( BOOST_PP_ENUM_PARAMS(N1, _) )\
        );\
    }\
/**/
BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_ARITY),
    BOOST_ASSIGN_V2_MACRO2,
    ~
)
#undef BOOST_ASSIGN_V2_MACRO1
#undef BOOST_ASSIGN_V2_MACRO2

#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

        pars_cont_type const& pars()const
        {
            return this->pars_cont;
        }
        seq_args_cont_type const& seq_args()const
        {
            return this->seq_args_cont;
        }

        protected:

        pars_cont_type pars_cont;
        seq_args_cont_type seq_args_cont;

    };

}// put_pipe_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_PIPE_CONTAINER_ER_2010_HPP
