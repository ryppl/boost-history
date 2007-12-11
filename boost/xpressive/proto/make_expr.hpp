///////////////////////////////////////////////////////////////////////////////
/// \file make_expr.hpp
/// Given a Fusion sequence of arguments and the type of a proto Expression,
/// unpacks the sequence into the Expression.
//
//  Copyright 2007 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_MAKE_EXPR_HPP_EAN_04_01_2005
#define BOOST_PROTO_MAKE_EXPR_HPP_EAN_04_01_2005

#include <boost/version.hpp>
#include <boost/ref.hpp>
#include <boost/mpl/if.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/xpressive/proto/proto_fwd.hpp>
#include <boost/xpressive/proto/traits.hpp>
#include <boost/xpressive/proto/domain.hpp>
#include <boost/xpressive/proto/generate.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/fusion/include/value_at.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/fusion/include/fold.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/fusion/include/transform_view.hpp>
#include <boost/fusion/include/invoke_function_object.hpp>

#define CV(T)\
    typename add_const<T>::type

#define REF(T)\
    typename add_reference<T>::type

#define CVREF(T)\
    REF(CV(T))

#define UNCV(T)\
    typename remove_cv<T>::type

#define UNREF(T)\
    typename remove_reference<T>::type

#define UNCVREF(T)\
    UNCV(UNREF(T))

/// INTERNAL ONLY
///
#define BOOST_PROTO_VARARG_TEMPLATE_AUX_(R, DATA, I, ELEM)                                      \
    (ELEM BOOST_PP_CAT(BOOST_PP_CAT(X, DATA), BOOST_PP_CAT(_, I)))                              \
    /**/

/// INTERNAL ONLY
///
#define BOOST_PROTO_VARARG_TEMPLATE_YES_(R, DATA, I, ELEM)                                      \
    BOOST_PP_LIST_FOR_EACH_I_R(                                                                 \
        R                                                                                       \
      , BOOST_PROTO_VARARG_TEMPLATE_AUX_                                                        \
      , I                                                                                       \
      , BOOST_PP_TUPLE_TO_LIST(                                                                 \
            BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(ELEM))                                               \
          , BOOST_PP_SEQ_TO_TUPLE(BOOST_PP_SEQ_TAIL(ELEM))                                      \
        )                                                                                       \
    )                                                                                           \
    /**/

/// INTERNAL ONLY
///
#define BOOST_PROTO_VARARG_TEMPLATE_NO_(R, DATA, I, ELEM)                                       \
    /**/

/// INTERNAL ONLY
///
#define BOOST_PROTO_VARARG_TEMPLATE_(R, DATA, I, ELEM)                                          \
    BOOST_PP_IF(                                                                                \
        BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(ELEM))                                                   \
      , BOOST_PROTO_VARARG_TEMPLATE_YES_                                                        \
      , BOOST_PROTO_VARARG_TEMPLATE_NO_                                                         \
    )(R, DATA, I, ELEM)                                                                         \
    /**/

/// INTERNAL ONLY
///
#define BOOST_PROTO_VARARG_TYPE_AUX_(R, DATA, I, ELEM)                                          \
    (BOOST_PP_CAT(BOOST_PP_CAT(X, DATA), BOOST_PP_CAT(_, I)))                                   \
    /**/

/// INTERNAL ONLY
///
#define BOOST_PROTO_TEMPLATE_PARAMS_YES_(R, DATA, I, ELEM)                                      \
    <                                                                                           \
        BOOST_PP_SEQ_ENUM(                                                                      \
            BOOST_PP_LIST_FOR_EACH_I_R(                                                         \
                R                                                                               \
              , BOOST_PROTO_VARARG_TYPE_AUX_                                                    \
              , I                                                                               \
              , BOOST_PP_TUPLE_TO_LIST(                                                         \
                    BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(ELEM))                                       \
                  , BOOST_PP_SEQ_TO_TUPLE(BOOST_PP_SEQ_TAIL(ELEM))                              \
                )                                                                               \
            )                                                                                   \
        )                                                                                       \
    >                                                                                           \
    /**/

/// INTERNAL ONLY
///
#define BOOST_PROTO_TEMPLATE_PARAMS_NO_(R, DATA, I, ELEM)                                       \
    /**/

/// INTERNAL ONLY
///
#define BOOST_PROTO_VARARG_TYPE_(R, DATA, I, ELEM)                                              \
    BOOST_PP_COMMA_IF(I)                                                                        \
    BOOST_PP_SEQ_HEAD(ELEM)                                                                     \
    BOOST_PP_IF(                                                                                \
        BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(ELEM))                                                   \
      , BOOST_PROTO_TEMPLATE_PARAMS_YES_                                                        \
      , BOOST_PROTO_TEMPLATE_PARAMS_NO_                                                         \
    )(R, DATA, I, ELEM) BOOST_PP_EXPR_IF(BOOST_PP_GREATER(I, 1), const)                         \
    /**/

/// INTERNAL ONLY
///
#define BOOST_PROTO_VARARG_AS_EXPR_(R, DATA, I, ELEM)                                           \
    BOOST_PP_EXPR_IF(                                                                           \
        BOOST_PP_GREATER(I, 1)                                                                  \
      , ((                                                                                      \
            BOOST_PP_SEQ_HEAD(ELEM)                                                             \
            BOOST_PP_IF(                                                                        \
                BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(ELEM))                                           \
              , BOOST_PROTO_TEMPLATE_PARAMS_YES_                                                \
              , BOOST_PROTO_TEMPLATE_PARAMS_NO_                                                 \
            )(R, DATA, I, ELEM)()                                                               \
        ))                                                                                      \
    )                                                                                           \
    /**/

/// INTERNAL ONLY
///
#define BOOST_PROTO_VARARG_AS_ARG_(Z, N, DATA)                                                  \
    (BOOST_PP_CAT(DATA, N))                                                                     \
    /**/

/// INTERNAL ONLY
///
#define BOOST_PROTO_SEQ_PUSH_FRONT(SEQ, ELEM)                                                   \
    BOOST_PP_SEQ_POP_BACK(BOOST_PP_SEQ_PUSH_FRONT(BOOST_PP_SEQ_PUSH_BACK(SEQ, _dummy_), ELEM))  \
    /**/

/// INTERNAL ONLY
///
#define BOOST_PROTO_VARARG_AS_PARAM_(Z, N, DATA)                                                \
    (BOOST_PP_CAT(DATA, N))                                                                     \
    /**/

/// INTERNAL ONLY
///
#define BOOST_PROTO_VARARG_FUN_(Z, N, DATA)                                                     \
    template<                                                                                   \
        BOOST_PP_SEQ_ENUM(                                                                      \
            BOOST_PP_SEQ_FOR_EACH_I(                                                            \
                BOOST_PROTO_VARARG_TEMPLATE_, ~                                                 \
              , BOOST_PP_SEQ_PUSH_FRONT(                                                        \
                    BOOST_PROTO_SEQ_PUSH_FRONT(                                                 \
                        BOOST_PP_TUPLE_ELEM(4, 2, DATA)                                         \
                      , (BOOST_PP_TUPLE_ELEM(4, 3, DATA))                                       \
                    )                                                                           \
                  , BOOST_PP_TUPLE_ELEM(4, 1, DATA)                                             \
                )                                                                               \
            )                                                                                   \
            BOOST_PP_REPEAT_ ## Z(N, BOOST_PROTO_VARARG_AS_PARAM_, typename A)                  \
        )                                                                                       \
    >                                                                                           \
    typename boost::proto::result_of::make_expr<                                                \
        BOOST_PP_SEQ_FOR_EACH_I(                                                                \
            BOOST_PROTO_VARARG_TYPE_, ~                                                         \
          , BOOST_PP_SEQ_PUSH_FRONT(                                                            \
                BOOST_PROTO_SEQ_PUSH_FRONT(                                                     \
                    BOOST_PP_TUPLE_ELEM(4, 2, DATA)                                             \
                  , (BOOST_PP_TUPLE_ELEM(4, 3, DATA))                                           \
                )                                                                               \
              , BOOST_PP_TUPLE_ELEM(4, 1, DATA)                                                 \
            )                                                                                   \
        )                                                                                       \
        BOOST_PP_ENUM_TRAILING_PARAMS(N, const A)                                               \
    >::type                                                                                     \
    BOOST_PP_TUPLE_ELEM(4, 0, DATA)(BOOST_PP_ENUM_BINARY_PARAMS_Z(Z, N, const A, &a))           \
    {                                                                                           \
        return boost::proto::result_of::make_expr<                                              \
            BOOST_PP_SEQ_FOR_EACH_I(                                                            \
                BOOST_PROTO_VARARG_TYPE_, ~                                                     \
              , BOOST_PP_SEQ_PUSH_FRONT(                                                        \
                    BOOST_PROTO_SEQ_PUSH_FRONT(                                                 \
                        BOOST_PP_TUPLE_ELEM(4, 2, DATA)                                         \
                      , (BOOST_PP_TUPLE_ELEM(4, 3, DATA))                                       \
                    )                                                                           \
                  , BOOST_PP_TUPLE_ELEM(4, 1, DATA)                                             \
                )                                                                               \
            )                                                                                   \
            BOOST_PP_ENUM_TRAILING_PARAMS(N, const A)                                           \
        >::call(                                                                                \
            BOOST_PP_SEQ_ENUM(                                                                  \
                BOOST_PP_SEQ_FOR_EACH_I(                                                        \
                    BOOST_PROTO_VARARG_AS_EXPR_, ~                                              \
                  , BOOST_PP_SEQ_PUSH_FRONT(                                                    \
                        BOOST_PROTO_SEQ_PUSH_FRONT(                                             \
                            BOOST_PP_TUPLE_ELEM(4, 2, DATA)                                     \
                          , (BOOST_PP_TUPLE_ELEM(4, 3, DATA))                                   \
                        )                                                                       \
                      , BOOST_PP_TUPLE_ELEM(4, 1, DATA)                                         \
                    )                                                                           \
                )                                                                               \
                BOOST_PP_REPEAT_ ## Z(N, BOOST_PROTO_VARARG_AS_ARG_, a)                         \
            )                                                                                   \
        );                                                                                      \
    }                                                                                           \
    /**/

/// \code
/// BOOST_PROTO_DEFINE_FUNCTION_TEMPLATE(
///     1
///   , construct
///   , boost::proto::default_domain
///   , (boost::proto::tag::function)
///   , ((op::construct)(typename)(int))
/// )
/// \endcode
#define BOOST_PROTO_DEFINE_FUNCTION_TEMPLATE(ARGCOUNT, NAME, DOMAIN, TAG, BOUNDARGS)            \
    BOOST_PP_REPEAT_FROM_TO(                                                                    \
        ARGCOUNT                                                                                \
      , BOOST_PP_INC(ARGCOUNT)                                                                  \
      , BOOST_PROTO_VARARG_FUN_                                                                 \
      , (NAME, TAG, BOUNDARGS, DOMAIN)                                                          \
    )\
    /**/

/// \code
/// BOOST_PROTO_DEFINE_VARARG_FUNCTION_TEMPLATE(
///     construct
///   , boost::proto::default_domain
///   , (boost::proto::tag::function)
///   , ((op::construct)(typename)(int))
/// )
/// \endcode
#define BOOST_PROTO_DEFINE_VARARG_FUNCTION_TEMPLATE(NAME, DOMAIN, TAG, BOUNDARGS)               \
    BOOST_PP_REPEAT(                                                                            \
        BOOST_PP_SUB(BOOST_PP_INC(BOOST_PROTO_MAX_ARITY), BOOST_PP_SEQ_SIZE(BOUNDARGS))         \
      , BOOST_PROTO_VARARG_FUN_                                                                 \
      , (NAME, TAG, BOUNDARGS, DOMAIN)                                                          \
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
        template<typename Domain, typename... Rest>
        struct deduce_domain_
        {
            typedef Domain type;
        };

        template<typename Head, typename... Tail>
        struct deduce_domain_<default_domain, Head, Tail...>
          : deduce_domain_<typename domain_of<Head>::type, Tail...>
        {};

        struct fold_domain_
        {
            template<typename Sig>
            struct result
            {};

            template<typename This, typename Domain, typename State>
            struct result<This(Domain, State)>
              : mpl::if_<
                    is_same<UNCVREF(Domain), default_domain>
                  , UNCVREF(State)
                  , UNCVREF(Domain)
                >
            {};
        };

        struct domain_of_
        {
            template<typename Sig>
            struct result
            {};

            template<typename This, typename Domain>
            struct result<This(Domain)>
              : domain_of<UNCVREF(Domain)>
            {};
        };

        struct make_args_fun
        {
            template<typename Sig>
            struct result
            {};

            template<typename This, typename... Args>
            struct result<This(Args...)>
            {
                typedef args<UNCV(Args)...> type;
            };
        };

        template<
            typename Tag
          , typename Domain
          , template<typename> class AsExpr
          , typename Sequence
          , std::size_t Size
        >
        struct unpack_
        {
            typedef expr<
                Tag
              , typename fusion::result_of::invoke_function_object<
                    make_args_fun
                  , fusion::transform_view<Sequence const, AsExpr<Domain> >
                >::type
            > expr_type;

            typedef typename Domain::template apply<expr_type>::type type;

            static type call(CVREF(Sequence) sequence)
            {
                fusion::transform_view<Sequence const, AsExpr<Domain> > seq(sequence, AsExpr<Domain>());

                expr_type that = {
                    fusion::invoke_function_object(argsns_::make_cons_fun(), seq)
                };

                return Domain::make(that);
            }
        };

        template<
            typename Domain
          , template<typename> class AsExpr
          , typename Sequence
        >
        struct unpack_<tag::terminal, Domain, AsExpr, Sequence, 1u>
        {
            typedef
                typename fusion::result_of::value_of<
                    typename fusion::result_of::begin<Sequence>::type
                >::type
            value_type;

            typedef typename boost::result_of<AsExpr<Domain>(value_type)>::type type;

            static type call(CVREF(Sequence) sequence)
            {
                return AsExpr<Domain>()(fusion::front(sequence));
            }
        };

        template<
            typename Tag
          , template<typename> class AsExpr
          , typename Sequence
          , std::size_t N
        >
        struct unpack_<Tag, deduce_domain, AsExpr, Sequence, N>
          : unpack_<
                Tag
              , typename fusion::result_of::fold<
                    fusion::transform_view<Sequence, proto::detail::domain_of_>
                  , default_domain
                  , proto::detail::fold_domain_
                >::type
              , AsExpr
              , Sequence
              , N
            >
        {};

        template<
            template<typename> class AsExpr
          , typename Sequence
        >
        struct unpack_<tag::terminal, deduce_domain, AsExpr, Sequence, 1u>
          : unpack_<tag::terminal, default_domain, AsExpr, Sequence, 1u>
        {};


        template<typename Tag, typename Domain, template<typename> class AsExpr, typename... Args>
        struct make_
        {
            typedef
                expr<
                    Tag
                  , args<typename boost::result_of<AsExpr<Domain>(Args)>::type...>
                >
            expr_type;

            typedef typename Domain::template apply<expr_type>::type type;

            static type call(CVREF(Args)... args)
            {
                return Domain::make(expr_type::make(AsExpr<Domain>()(args)...));
            }
        };

        template<typename Domain, template<typename> class AsExpr, typename A>
        struct make_<tag::terminal, Domain, AsExpr, A>
        {
            typedef typename boost::result_of<AsExpr<Domain>(A)>::type type;

            static type call(CVREF(A) a)
            {
                return AsExpr<Domain>()(a);
            }
        };

    }

    namespace result_of
    {
        template<typename Tag, typename Sequence, typename, typename>
        struct unpack_arg
          : proto::detail::unpack_<
                Tag
              , deduce_domain
              , functional::as_arg
              , Sequence
              , fusion::result_of::size<Sequence>::type::value
            >
        {};

        template<typename Tag, typename Domain, typename Sequence>
        struct unpack_arg<Tag, Domain, Sequence, typename Domain::proto_is_domain_>
          : proto::detail::unpack_<
                Tag
              , Domain
              , functional::as_arg
              , Sequence
              , fusion::result_of::size<Sequence>::type::value
            >
        {};

        template<typename Tag, typename Head, typename... Tail>
        struct make_arg<Tag, Head, Tail...>
          : mpl::if_<
                is_domain<Head>
              , proto::detail::make_<Tag, Head, functional::as_arg, Tail...>
              , make_arg<Tag, deduce_domain, Head, Tail...>
            >::type
        {};

        template<typename Tag, typename Head, typename... Tail>
        struct make_arg<Tag, deduce_domain, Head, Tail...>
          : proto::detail::make_<
                Tag
              , typename proto::detail::deduce_domain_<
                    typename domain_of<Head>::type
                  , Tail...
                >::type
              , functional::as_arg
              , Head
              , Tail...
            >
        {};

        template<typename Tag, typename Sequence, typename, typename>
        struct unpack_expr
          : proto::detail::unpack_<
                Tag
              , deduce_domain
              , functional::as_expr
              , Sequence
              , fusion::result_of::size<Sequence>::type::value
            >
        {};

        template<typename Tag, typename Domain, typename Sequence>
        struct unpack_expr<Tag, Domain, Sequence, typename Domain::proto_is_domain_>
          : proto::detail::unpack_<
                Tag
              , Domain
              , functional::as_expr
              , Sequence
              , fusion::result_of::size<Sequence>::type::value
            >
        {};

        template<typename Tag, typename Head, typename... Tail>
        struct make_expr<Tag, Head, Tail...>
          : mpl::if_<
                is_domain<Head>
              , proto::detail::make_<Tag, Head, functional::as_expr, Tail...>
              , make_expr<Tag, deduce_domain, Head, Tail...>
            >::type
        {};

        template<typename Tag, typename Head, typename... Tail>
        struct make_expr<Tag, deduce_domain, Head, Tail...>
          : proto::detail::make_<
                Tag
              , typename proto::detail::deduce_domain_<
                    typename domain_of<Head>::type
                  , Tail...
                >::type
              , functional::as_expr
              , Head
              , Tail...
            >
        {};
    }

    namespace functional
    {
        template<typename Tag, typename Domain>
        struct make_arg
        {
            BOOST_PROTO_CALLABLE()

            template<typename Sig>
            struct result
            {};

            template<typename This, typename... A>
            struct result<This(A...)>
              : result_of::make_arg<Tag, Domain, A...>
            {};

            template<typename... A>
            typename result_of::make_arg<Tag, Domain, A...>::type
            operator ()(A &&...a) const
            {
                return result_of::make_arg<Tag, Domain, A...>::call(a...);
            }
        };

        template<typename Tag, typename Domain>
        struct unpack_arg
        {
            BOOST_PROTO_CALLABLE()

            template<typename Sig>
            struct result
            {};

            template<typename This, typename Sequence>
            struct result<This(Sequence)>
              : result_of::unpack_arg<Tag, Domain, UNCVREF(Sequence)>
            {};

            template<typename Sequence>
            typename result_of::unpack_arg<Tag, Domain, Sequence>::type
            operator ()(Sequence const &sequence) const
            {
                return result_of::unpack_arg<Tag, Domain, Sequence>::call(sequence);
            }
        };

        template<typename Tag, typename Domain>
        struct unfused_arg_fun
        {
            BOOST_PROTO_CALLABLE()

            template<typename Sequence>
            struct result
              : result_of::unpack_arg<Tag, Domain, Sequence>
            {};

            template<typename Sequence>
            typename proto::result_of::unpack_arg<Tag, Domain, Sequence>::type
            operator ()(Sequence const &sequence) const
            {
                return result_of::unpack_arg<Tag, Domain, Sequence>::call(sequence);
            }
        };

        template<typename Tag, typename Domain>
        struct unfused_arg
          : fusion::unfused_generic<unfused_arg_fun<Tag, Domain> >
        {
            BOOST_PROTO_CALLABLE()
        };

        template<typename Tag, typename Domain>
        struct make_expr
        {
            BOOST_PROTO_CALLABLE()

            template<typename Sig>
            struct result
            {};

            template<typename This, typename... A>
            struct result<This(A...)>
              : result_of::make_expr<Tag, Domain, A...>
            {};

            template<typename... A>
            typename result_of::make_expr<Tag, Domain, A...>::type
            operator ()(A &&...a) const
            {
                return result_of::make_expr<Tag, Domain, A...>::call(a...);
            }
        };

        template<typename Tag, typename Domain>
        struct unpack_expr
        {
            BOOST_PROTO_CALLABLE()

            template<typename Sig>
            struct result
            {};

            template<typename This, typename Sequence>
            struct result<This(Sequence)>
              : result_of::unpack_expr<Tag, Domain, UNCVREF(Sequence)>
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
            BOOST_PROTO_CALLABLE()

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
        {
            BOOST_PROTO_CALLABLE()
        };
    }

    /// unpack_arg
    ///
    template<typename Tag, typename Sequence>
    typename lazy_disable_if<
        is_domain<Sequence>
      , result_of::unpack_arg<Tag, Sequence>
    >::type
    unpack_arg(Sequence const &sequence)
    {
        return result_of::unpack_arg<Tag, Sequence>::call(sequence);
    }

    /// \overload
    ///
    template<typename Tag, typename Domain, typename Sequence2>
    typename result_of::unpack_arg<Tag, Domain, Sequence2>::type
    unpack_arg(Sequence2 const &sequence2)
    {
        return result_of::unpack_arg<Tag, Domain, Sequence2>::call(sequence2);
    }

    /// make_arg
    ///
    template<typename Tag, typename Head, typename... Tail>
    typename lazy_disable_if<
        is_domain<Head>
      , result_of::make_arg<Tag, Head, Tail...>
    >::type
    make_arg(Head &&head, Tail &&... tail)
    {
        return result_of::make_arg<Tag, Head, Tail...>::call(head, tail...);
    }

    /// \overload
    ///
    template<typename Tag, typename Domain, typename Head, typename... Tail>
    typename result_of::make_arg<Tag, Domain, Head, Tail...>::type
    make_arg(Head &&head, Tail &&... tail)
    {
        return result_of::make_arg<Tag, Domain, Head, Tail...>::call(head, tail...);
    }

    /// unpack_expr
    ///
    template<typename Tag, typename Sequence>
    typename lazy_disable_if<
        is_domain<Sequence>
      , result_of::unpack_expr<Tag, Sequence>
    >::type
    unpack_expr(Sequence const &sequence)
    {
        return result_of::unpack_expr<Tag, Sequence>::call(sequence);
    }

    /// \overload
    ///
    template<typename Tag, typename Domain, typename Sequence2>
    typename result_of::unpack_expr<Tag, Domain, Sequence2>::type
    unpack_expr(Sequence2 const &sequence2)
    {
        return result_of::unpack_expr<Tag, Domain, Sequence2>::call(sequence2);
    }

    /// make_expr
    ///
    template<typename Tag, typename Head, typename... Tail>
    typename lazy_disable_if<
        is_domain<Head>
      , result_of::make_expr<Tag, Head, Tail...>
    >::type
    make_expr(Head &&head, Tail &&... tail)
    {
        return result_of::make_expr<Tag, Head, Tail...>::call(head, tail...);
    }

    /// \overload
    ///
    template<typename Tag, typename Domain, typename Head, typename... Tail>
    typename result_of::make_expr<Tag, Domain, Head, Tail...>::type
    make_expr(Head &&head, Tail &&... tail)
    {
        return result_of::make_expr<Tag, Domain, Head, Tail...>::call(head, tail...);
    }


    template<typename Tag, typename Domain>
    struct is_callable<functional::make_arg<Tag, Domain> >
      : mpl::true_
    {};

    template<typename Tag, typename Domain>
    struct is_callable<functional::unpack_arg<Tag, Domain> >
      : mpl::true_
    {};

    template<typename Tag, typename Domain>
    struct is_callable<functional::unfused_arg<Tag, Domain> >
      : mpl::true_
    {};

    template<typename Tag, typename Domain>
    struct is_callable<functional::make_expr<Tag, Domain> >
      : mpl::true_
    {};

    template<typename Tag, typename Domain>
    struct is_callable<functional::unpack_expr<Tag, Domain> >
      : mpl::true_
    {};

    template<typename Tag, typename Domain>
    struct is_callable<functional::unfused_expr<Tag, Domain> >
      : mpl::true_
    {};


}}

#undef CV
#undef REF
#undef CVREF
#undef UNCV
#undef UNREF
#undef UNCVREF

#endif
