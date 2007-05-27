#ifndef BOOST_PP_IS_ITERATING
    ///////////////////////////////////////////////////////////////////////////////
    /// \file make_expr.hpp
    /// Given a Fusion sequence of arguments and the type of a proto Expression,
    /// unpacks the sequence into the Expression.
    //
    //  Copyright 2004 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #ifndef BOOST_PROTO_MAKE_EXPR_HPP_EAN_04_01_2005
    #define BOOST_PROTO_MAKE_EXPR_HPP_EAN_04_01_2005

    #include <boost/xpressive/proto/detail/prefix.hpp>
    #include <boost/preprocessor/cat.hpp>
    #include <boost/preprocessor/arithmetic/dec.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/preprocessor/iterate.hpp>
    #include <boost/preprocessor/facilities/intercept.hpp>
    #include <boost/preprocessor/tuple/elem.hpp>
    #include <boost/preprocessor/repetition/enum.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/enum_trailing_params.hpp>
    #include <boost/preprocessor/repetition/enum_binary_params.hpp>
    #include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
    #include <boost/preprocessor/repetition/enum_shifted_params.hpp>
    #include <boost/ref.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/type_traits/is_same.hpp>
    #include <boost/type_traits/add_reference.hpp>
    #include <boost/type_traits/remove_reference.hpp>
    #include <boost/xpressive/proto/proto_fwd.hpp>
    #include <boost/xpressive/proto/traits.hpp>
    #include <boost/xpressive/proto/domain.hpp>
    #include <boost/xpressive/proto/generate.hpp>
    #include <boost/fusion/sequence/intrinsic/at.hpp>
    #include <boost/fusion/sequence/intrinsic/value_at.hpp>
    #include <boost/fusion/sequence/intrinsic/size.hpp>
    #include <boost/xpressive/proto/detail/suffix.hpp>

    #define BOOST_PROTO_AS_ARG_TYPE(Z, N, DATA)                                                     \
        typename proto::result_of::as_arg<                                                          \
            BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 0, DATA), N)                                        \
          , BOOST_PP_TUPLE_ELEM(2, 1, DATA)                                                         \
        >::type                                                                                     \
        /**/

    #define BOOST_PROTO_AS_ARG(Z, N, DATA)                                                          \
        proto::as_arg<BOOST_PP_TUPLE_ELEM(2, 1, DATA) >(                                            \
            BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 0, DATA), N)                                        \
        )                                                                                           \
        /**/

    #define BOOST_PROTO_AT_TYPE(Z, N, DATA)                                                         \
        typename remove_reference<                                                                  \
            typename fusion::result_of::value_at_c<BOOST_PP_TUPLE_ELEM(2, 0, DATA), N >::type       \
        >::type                                                                                     \
        /**/

    #define BOOST_PROTO_AT(Z, N, DATA)                                                              \
        fusion::at_c<N >(BOOST_PP_TUPLE_ELEM(2, 0, DATA))                                           \
        /**/

    #define BOOST_PROTO_AS_ARG_AT_TYPE(Z, N, DATA)                                                  \
        typename result_of::as_arg<                                                                 \
            BOOST_PROTO_AT_TYPE(Z, N, DATA)                                                         \
          , BOOST_PP_TUPLE_ELEM(2, 1, DATA)                                                         \
        >::type                                                                                     \
        /**/

    #define BOOST_PROTO_AS_ARG_AT(Z, N, DATA)                                                       \
        proto::as_arg<BOOST_PP_TUPLE_ELEM(2, 1, DATA) >(                                            \
            BOOST_PROTO_AT(Z, N, DATA)                                                              \
        )                                                                                           \
        /**/

    namespace boost { namespace fusion
    {
        template<typename Function>
        class unfused_generic;
    }}

    namespace boost { namespace proto
    {
        namespace detail
        {
            template<
                typename Domain
                BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(
                    BOOST_PROTO_MAX_ARITY
                  , typename A
                  , = default_domain BOOST_PP_INTERCEPT
                )
            >
            struct deduce_domain_
            {
                typedef Domain type;
            };

            template<BOOST_PP_ENUM_PARAMS(BOOST_PROTO_MAX_ARITY, typename A)>
            struct deduce_domain_<
                default_domain
                BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PROTO_MAX_ARITY, A)
            >
              : deduce_domain_<
                    typename domain_of<A0>::type
                  , BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PROTO_MAX_ARITY, typename A)
                >
            {};

            template<>
            struct deduce_domain_<default_domain>
            {
                typedef default_domain type;
            };

            template<typename Tag, typename Domain, typename Sequence, std::size_t Size>
            struct unpack_expr_
            {};

            template<typename Domain, typename Sequence>
            struct unpack_expr_<tag::terminal, Domain, Sequence, 1u>
            {
                typedef expr<
                    tag::terminal
                  , args1<typename fusion::result_of::value_at_c<Sequence, 0>::type>
                > expr_type;

                typedef typename Domain::template apply<expr_type>::type type;

                static type const call(Sequence const &sequence)
                {
                    expr_type that = {fusion::at_c<0>(sequence)};
                    return Domain::make(that);
                }
            };

            template<
                typename Tag
              , typename Domain
                BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(BOOST_PROTO_MAX_ARITY, typename A, = void BOOST_PP_INTERCEPT)
              , typename _ = void
            >
            struct make_expr_
            {};

            template<typename Domain, typename A>
            struct make_expr_<tag::terminal, Domain, A>
            {
                typedef typename add_reference<A>::type reference;
                typedef expr<tag::terminal, args1<reference> > expr_type;
                typedef typename Domain::template apply<expr_type>::type type;

                static type const call(reference a)
                {
                    expr_type that = {a};
                    return Domain::make(that);
                }
            };

        #define BOOST_PP_ITERATION_PARAMS_1                                                         \
            (4, (1, BOOST_PROTO_MAX_ARITY, <boost/xpressive/proto/make_expr.hpp>, 1))               \
            /**/

        #include BOOST_PP_ITERATE()
        }

        namespace result_of
        {
            template<typename Tag, typename Sequence, typename, typename>
            struct unpack_expr
              : detail::unpack_expr_<Tag, deduce_domain, Sequence, fusion::result_of::size<Sequence>::type::value>
            {};

            template<typename Tag, typename Domain, typename Sequence>
            struct unpack_expr<Tag, Domain, Sequence, typename Domain::boost_proto_is_domain_>
              : detail::unpack_expr_<Tag, Domain, Sequence, fusion::result_of::size<Sequence>::type::value>
            {};

            template<typename Tag BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PROTO_MAX_ARITY, typename A), typename, typename>
            struct make_expr
              : make_expr<Tag, deduce_domain BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PROTO_MAX_ARITY, A) >
            {};

            template<typename Tag, typename Domain BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PROTO_MAX_ARITY, typename A) >
            struct make_expr<Tag, Domain BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PROTO_MAX_ARITY, A), typename Domain::boost_proto_is_domain_>
              : detail::make_expr_<Tag, Domain BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PROTO_MAX_ARITY, A) >
            {};

            template<typename Tag BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PROTO_MAX_ARITY, typename A) >
            struct make_expr<Tag, deduce_domain BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PROTO_MAX_ARITY, A), void>
              : detail::make_expr_<
                    Tag
                  , typename detail::deduce_domain_<
                        typename domain_of<A0>::type
                      , BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PROTO_MAX_ARITY, A)
                    >::type
                    BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PROTO_MAX_ARITY, A)
                >
            {};
        }

        namespace functional
        {
            template<typename Tag, typename Domain>
            struct make_expr
            {
                template<typename Sig>
                struct result
                {};

                template<typename A>
                typename result_of::make_expr<Tag, Domain, A>::type const
                operator ()(A &a) const
                {
                    return result_of::make_expr<Tag, Domain, A>::call(a);
                }

        #define BOOST_PP_ITERATION_PARAMS_1                                                         \
            (4, (1, BOOST_PROTO_MAX_ARITY, <boost/xpressive/proto/make_expr.hpp>, 2))               \
            /**/

        #include BOOST_PP_ITERATE()
            };

            template<typename Domain>
            struct make_expr<tag::terminal, Domain>
            {
                template<typename Sig>
                struct result
                {};

                template<typename This, typename A>
                struct result<This(A)>
                  : result_of::make_expr<tag::terminal, Domain, A>
                {};

                template<typename A>
                typename result_of::make_expr<tag::terminal, Domain, A>::type operator ()(A &a) const
                {
                    return result_of::make_expr<tag::terminal, Domain, A>::call(a);
                }

                template<typename A>
                typename result_of::make_expr<tag::terminal, Domain, A const>::type operator ()(A const &a) const
                {
                    return result_of::make_expr<tag::terminal, Domain, A const>::call(a);
                }
            };

            template<typename Tag, typename Domain>
            struct unpack_expr
            {
                template<typename Sig>
                struct result
                {};

                template<typename This, typename Sequence>
                struct result<This(Sequence)>
                  : result_of::unpack_expr<Tag, Domain, typename detail::remove_cv_ref<Sequence>::type>
                {};

                template<typename Sequence>
                typename result_of::unpack_expr<Tag, Domain, Sequence>::type
                operator ()(Sequence const &sequence) const
                {
                    return result_of::unpack_expr<Tag, Domain, Sequence>::call(sequence);
                }
            };

            template<typename Tag, typename Domain>
            struct unfused_expr_fun
            {
                template<typename Sequence>
                struct result
                  : result_of::unpack_expr<Tag, Domain, Sequence>
                {};

                template<typename Sequence>
                typename proto::result_of::unpack_expr<Tag, Domain, Sequence>::type
                operator ()(Sequence const &sequence) const
                {
                    return result_of::unpack_expr<Tag, Domain, Sequence>::call(sequence);
                }
            };

            template<typename Tag, typename Domain>
            struct unfused_expr
              : fusion::unfused_generic<unfused_expr_fun<Tag, Domain> >
            {};
        }

        /// unpack_expr
        ///
        template<typename Tag, typename Sequence>
        typename lazy_disable_if<
            is_domain<Sequence>
          , result_of::unpack_expr<Tag, Sequence>
        >::type const
        unpack_expr(Sequence const &sequence)
        {
            return result_of::unpack_expr<Tag, Sequence>::call(sequence);
        }

        /// \overload
        ///
        template<typename Tag, typename Domain, typename Sequence2>
        typename result_of::unpack_expr<Tag, Domain, Sequence2>::type const
        unpack_expr(Sequence2 const &sequence2)
        {
            return result_of::unpack_expr<Tag, Domain, Sequence2>::call(sequence2);
        }

        /// make_expr
        ///
        template<typename Tag, typename A0>
        typename lazy_disable_if<
            is_domain<A0>
          , result_of::make_expr<Tag, A0>
        >::type const
        make_expr(A0 &a0 BOOST_PROTO_DISABLE_IF_IS_CONST(A0))
        {
            return result_of::make_expr<Tag, A0>::call(a0);
        }

        /// \overload
        ///
        template<typename Tag, typename Domain, typename B0>
        typename result_of::make_expr<Tag, Domain, B0>::type const
        make_expr(B0 &b0 BOOST_PROTO_DISABLE_IF_IS_CONST(B0))
        {
            return result_of::make_expr<Tag, Domain, B0>::call(b0);
        }

    #define BOOST_PP_ITERATION_PARAMS_1                                                             \
        (4, (1, BOOST_PROTO_MAX_ARITY, <boost/xpressive/proto/make_expr.hpp>, 3))                   \
        /**/

    #include BOOST_PP_ITERATE()
    }}

    #undef BOOST_PROTO_AT
    #undef BOOST_PROTO_AT_TYPE
    #undef BOOST_PROTO_AS_ARG
    #undef BOOST_PROTO_AS_ARG_TYPE
    #undef BOOST_PROTO_AS_ARG_AT
    #undef BOOST_PROTO_AS_ARG_AT_TYPE

    #endif // BOOST_PROTO_MAKE_EXPR_HPP_EAN_04_01_2005

#elif BOOST_PP_ITERATION_FLAGS() == 1

    #define N BOOST_PP_ITERATION()

        template<typename Tag, typename Domain BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
        struct make_expr_<Tag, Domain BOOST_PP_ENUM_TRAILING_PARAMS(N, A) >
        {
            typedef expr<
                Tag
              , BOOST_PP_CAT(args, N)<BOOST_PP_ENUM(N, BOOST_PROTO_AS_ARG_TYPE, (A, Domain)) >
            > expr_type;

            typedef typename Domain::template apply<expr_type>::type type;

            static type const call(BOOST_PP_ENUM_BINARY_PARAMS(N, A, &a))
            {
                expr_type that = {
                    BOOST_PP_ENUM(N, BOOST_PROTO_AS_ARG, (a, Domain))
                };
                return Domain::make(that);
            }
        };

        template<typename Tag, typename Domain, typename Sequence>
        struct unpack_expr_<Tag, Domain, Sequence, N>
        {
            typedef expr<
                Tag
              , BOOST_PP_CAT(args, N)<BOOST_PP_ENUM(N, BOOST_PROTO_AS_ARG_AT_TYPE, (Sequence const, Domain)) >
            > expr_type;

            typedef typename Domain::template apply<expr_type>::type type;

            static type const call(Sequence const &sequence)
            {
                expr_type that = {
                    BOOST_PP_ENUM(N, BOOST_PROTO_AS_ARG_AT, (sequence, Domain))
                };
                return Domain::make(that);
            }
        };

        template<typename Tag, typename Sequence>
        struct unpack_expr_<Tag, deduce_domain, Sequence, N>
          : unpack_expr_<
                Tag
              , typename detail::deduce_domain_<
                    typename domain_of<
                        BOOST_PROTO_AT_TYPE(~, 0, (Sequence const, ~))
                    >::type
                    BOOST_PP_COMMA_IF(BOOST_PP_DEC(N))
                    BOOST_PP_ENUM_SHIFTED(N, BOOST_PROTO_AT_TYPE, (Sequence const, ~))
                >::type
              , Sequence
              , N
            >
        {};

    #undef N

#elif BOOST_PP_ITERATION_FLAGS() == 2

    #define N BOOST_PP_ITERATION()

        template<typename This BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
        struct result<This(BOOST_PP_ENUM_PARAMS(N, A))>
          : result_of::make_expr<
                Tag
              , Domain
                BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(
                    N
                  , typename remove_reference<A
                  , >::type BOOST_PP_INTERCEPT
                )
            >
        {};

        template<BOOST_PP_ENUM_PARAMS(N, typename A)>
        typename result_of::make_expr<Tag, Domain BOOST_PP_ENUM_TRAILING_PARAMS(N, const A)>::type const
        operator ()(BOOST_PP_ENUM_BINARY_PARAMS(N, const A, &a)) const
        {
            return result_of::make_expr<Tag, Domain BOOST_PP_ENUM_TRAILING_PARAMS(N, const A)>::call(
                BOOST_PP_ENUM_PARAMS(N, a)
            );
        }

    #undef N

#elif BOOST_PP_ITERATION_FLAGS() == 3

    #define N BOOST_PP_ITERATION()

        /// \overload
        ///
        template<typename Tag BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
        typename lazy_disable_if<
            is_domain<A0>
          , result_of::make_expr<Tag BOOST_PP_ENUM_TRAILING_PARAMS(N, const A)>
        >::type const
        make_expr(BOOST_PP_ENUM_BINARY_PARAMS(N, const A, &a))
        {
            return result_of::make_expr<Tag BOOST_PP_ENUM_TRAILING_PARAMS(N, const A)>::call(
                BOOST_PP_ENUM_PARAMS(N, a)
            );
        }

        /// \overload
        ///
        template<typename Tag, typename Domain BOOST_PP_ENUM_TRAILING_PARAMS(N, typename B)>
        typename result_of::make_expr<Tag, Domain BOOST_PP_ENUM_TRAILING_PARAMS(N, const B)>::type const
        make_expr(BOOST_PP_ENUM_BINARY_PARAMS(N, const B, &b))
        {
            return result_of::make_expr<Tag, Domain BOOST_PP_ENUM_TRAILING_PARAMS(N, const B)>::call(
                BOOST_PP_ENUM_PARAMS(N, b)
            );
        }

    #undef N

#endif // BOOST_PP_IS_ITERATING
