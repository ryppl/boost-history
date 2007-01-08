///////////////////////////////////////////////////////////////////////////////
/// \file expr.hpp
/// Contains definition of expr\<\> class template.
//
//  Copyright 2004 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PP_IS_ITERATING
    #ifndef BOOST_PROTO_EXPR_HPP_EAN_04_01_2005
    #define BOOST_PROTO_EXPR_HPP_EAN_04_01_2005

    #include <boost/preprocessor/inc.hpp>
    #include <boost/preprocessor/dec.hpp>
    #include <boost/preprocessor/cat.hpp>
    #include <boost/preprocessor/iterate.hpp>
    #include <boost/preprocessor/punctuation/comma.hpp>
    #include <boost/preprocessor/facilities/intercept.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/repetition/repeat_from_to.hpp>
    #include <boost/preprocessor/repetition/enum_trailing.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/enum_binary_params.hpp>
    #include <boost/preprocessor/repetition/enum_trailing_params.hpp>
    #include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>

    #include <boost/utility/result_of.hpp>
    #include <boost/xpressive/proto/proto_fwd.hpp>
    #include <boost/xpressive/proto/ref.hpp>
    #include <boost/xpressive/proto/args.hpp>
    #include <boost/xpressive/proto/traits.hpp>

    namespace boost { namespace proto
    {
    #define BOOST_PROTO_ARG(z, n, data)\
        typedef typename Args::BOOST_PP_CAT(arg, n) BOOST_PP_CAT(BOOST_PP_CAT(arg, n), _type);\
        BOOST_PP_CAT(BOOST_PP_CAT(arg, n), _type) BOOST_PP_CAT(arg, n);\
        /**/

    #define BOOST_PROTO_VOID(z, n, data)\
        typedef void BOOST_PP_CAT(BOOST_PP_CAT(arg, n), _type);\
        /**/

    #define BOOST_PROTO_AS_OP(z, n, data)\
        proto::as_expr_ref(BOOST_PP_CAT(a,n))\
        /**/

    #define BOOST_PROTO_UNREF_ARG_TYPE(z, n, data)\
        typename meta::unref<typename Args::BOOST_PP_CAT(arg, n)>::reference\
        /**/

    #define BOOST_PROTO_UNREF_ARG(z, n, data)\
        proto::unref(this->BOOST_PP_CAT(arg, n))\
        /**/

    #define BOOST_PP_ITERATION_PARAMS_1 (4, (1, BOOST_PROTO_MAX_ARITY, <boost/xpressive/proto/expr.hpp>, 1))
    #include BOOST_PP_ITERATE()
    #undef BOOST_PP_ITERATION_PARAMS_1

    #undef BOOST_PROTO_ARG
    #undef BOOST_PROTO_VOID
    #undef BOOST_PROTO_AS_OP
    #undef BOOST_PROTO_UNREF_ARG_TYPE
    #undef BOOST_PROTO_UNREF_ARG
    }}

    #endif // BOOST_PROTO_EXPR_HPP_EAN_04_01_2005

#elif BOOST_PP_ITERATION_DEPTH() == 1 && BOOST_PP_ITERATION_FLAGS() < 2

        template<typename Tag, typename Args>
        struct expr<Tag, Args, BOOST_PP_ITERATION() >
        {
            typedef Tag tag_type;
            typedef expr type;
            typedef Args args_type;
            typedef void domain;
            typedef mpl::long_<BOOST_PP_ITERATION()> arity;
            typedef proto_expr_tag fusion_tag;
            typedef void is_boost_proto_expr_;
            typedef expr boost_proto_expr_type_;

            BOOST_PROTO_IDENTITY_TRANSFORM();
            BOOST_PP_REPEAT(BOOST_PP_ITERATION(), BOOST_PROTO_ARG, ~)
            BOOST_PP_REPEAT_FROM_TO(BOOST_PP_ITERATION(), BOOST_PROTO_MAX_ARITY, BOOST_PROTO_VOID, ~)

            expr const &cast() const
            {
                return *this;
            }

            template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename A)>
            static expr make(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, const &a))
            {
                expr that = {BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), a)};
                return that;
            }

            template<typename Fun>
            typename boost::result_of<Fun(Tag BOOST_PP_ENUM_TRAILING(BOOST_PP_ITERATION(), BOOST_PROTO_UNREF_ARG_TYPE, ~))>::type
            eval(Fun &fun) const
            {
                return fun(Tag() BOOST_PP_ENUM_TRAILING(BOOST_PP_ITERATION(), BOOST_PROTO_UNREF_ARG, ~));
            }

            template<typename Fun>
            typename boost::result_of<Fun const(Tag BOOST_PP_ENUM_TRAILING(BOOST_PP_ITERATION(), BOOST_PROTO_UNREF_ARG_TYPE, ~))>::type
            eval(Fun const &fun) const
            {
                return fun(Tag(), BOOST_PP_ENUM(BOOST_PP_ITERATION(), BOOST_PROTO_UNREF_ARG, ~));
            }

            template<typename A>
            expr<tag::assign, args2<ref<expr>, typename meta::as_expr_ref<A>::type> > const
            operator =(A const &a) const
            {
                expr<tag::assign, args2<ref<expr>, typename meta::as_expr_ref<A>::type> > that = {{*this}, proto::as_expr_ref(a)};
                return that;
            }

            template<typename A>
            expr<tag::subscript, args2<ref<expr>, typename meta::as_expr_ref<A>::type> > const
            operator [](A const &a) const
            {
                expr<tag::subscript, args2<ref<expr>, typename meta::as_expr_ref<A>::type> > that = {{*this}, proto::as_expr_ref(a)};
                return that;
            }

            template<typename Sig>
            struct result;

            template<typename This>
            struct result<This()>
            {
                typedef expr<tag::function, args1<ref<expr> > > type;
            };

            expr<tag::function, args1<ref<expr> > > const
            operator ()() const
            {
                expr<tag::function, args1<ref<expr> > > that = {{*this}};
                return that;
            }

    #define BOOST_PP_ITERATION_PARAMS_2 (3, (1, BOOST_PP_DEC(BOOST_PROTO_MAX_ARITY), <boost/xpressive/proto/expr.hpp>))
    #include BOOST_PP_ITERATE()
    #undef BOOST_PP_ITERATION_PARAMS_2
        };

#elif BOOST_PP_ITERATION_DEPTH() == 2

    #define N BOOST_PP_ITERATION()

        template<typename This BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
        struct result<This(BOOST_PP_ENUM_PARAMS(N, A))>
        {
            typedef expr<tag::function, BOOST_PP_CAT(args, BOOST_PP_INC(N))<ref<expr> BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N, typename meta::as_expr_ref<A, >::type BOOST_PP_INTERCEPT)> > type;
        };

        template<BOOST_PP_ENUM_PARAMS(N, typename A)>
        expr<tag::function, BOOST_PP_CAT(args, BOOST_PP_INC(N))<ref<expr> BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N, typename meta::as_expr_ref<A, >::type BOOST_PP_INTERCEPT)> > const
        operator ()(BOOST_PP_ENUM_BINARY_PARAMS(N, A, const &a)) const
        {
            expr<tag::function, BOOST_PP_CAT(args, BOOST_PP_INC(N))<ref<expr> BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N, typename meta::as_expr_ref<A, >::type BOOST_PP_INTERCEPT)> > that = {{*this} BOOST_PP_ENUM_TRAILING(N, BOOST_PROTO_AS_OP, _)};
            return that;
        }

    #undef N

#endif
