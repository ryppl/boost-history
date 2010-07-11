/*==============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/preprocessor/logical/not.hpp>
#   include <boost/preprocessor/comparison/equal.hpp>
#   include <boost/preprocessor/arithmetic/sub.hpp>
#   include <boost/preprocessor/arithmetic/dec.hpp>
#   include <boost/preprocessor/facilities/intercept.hpp>
#   include <boost/preprocessor/repetition/repeat.hpp>
#   include <boost/preprocessor/repetition/enum_params.hpp>
#   include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#   include <boost/preprocessor/punctuation/comma_if.hpp>
#   include <boost/mpl/pop_back.hpp>

namespace detail
{
    template<typename From, typename To>
    struct rebind_impl;

    template<typename From, typename To>
    struct rebind_next_impl;

#define BOOST_FUSION_TEST_REBIND_NEXT_IMPL(Z, N, _)                             \
    template<                                                                   \
        typename From                                                           \
      , template<BOOST_PP_ENUM_PARAMS(                                          \
            BOOST_FUSION_TEST_MAX_N, typename BOOST_PP_INTERCEPT)               \
        > class To                                                              \
        BOOST_PP_COMMA_IF(N)                                                    \
        BOOST_PP_ENUM_PARAMS(N, typename Arg)                                   \
    >                                                                           \
    struct rebind_next_impl<                                                    \
        From                                                                    \
      , To<                                                                     \
            BOOST_PP_ENUM_PARAMS(N,Arg)                                         \
            BOOST_PP_COMMA_IF(N)                                                \
            BOOST_PP_ENUM_PARAMS(                                               \
                BOOST_PP_SUB(BOOST_FUSION_TEST_MAX_N, N),                       \
                fusion::void_ BOOST_PP_INTERCEPT                                \
            )                                                                   \
        >                                                                       \
    >                                                                           \
      : rebind_impl<                                                            \
            typename mpl::pop_back<From>::type                                  \
          , To<                                                                 \
                BOOST_PP_ENUM_PARAMS(N,Arg)                                     \
                BOOST_PP_COMMA_IF(N)                                            \
                typename mpl::back<From>::type                                  \
                BOOST_PP_COMMA_IF(                                              \
                    BOOST_PP_SUB(BOOST_PP_DEC(BOOST_FUSION_TEST_MAX_N), N))     \
                BOOST_PP_ENUM_PARAMS(                                           \
                    BOOST_PP_SUB(BOOST_PP_DEC(BOOST_FUSION_TEST_MAX_N), N),     \
                    fusion::void_ BOOST_PP_INTERCEPT                            \
                )                                                               \
            >                                                                   \
        >                                                                       \
    {};

    BOOST_PP_REPEAT(
        BOOST_PP_DEC(BOOST_FUSION_TEST_MAX_N),
        BOOST_FUSION_TEST_REBIND_NEXT_IMPL,
        _)

#undef BOOST_FUSION_TEST_REBIND_NEXT_IMPL

    template<
        typename From
      , template<BOOST_PP_ENUM_PARAMS(
            BOOST_FUSION_TEST_MAX_N, typename BOOST_PP_INTERCEPT)> class To
      , BOOST_PP_ENUM_PARAMS(BOOST_FUSION_TEST_MAX_N, typename Arg)
    >
    struct rebind_impl<
        From
      , To<BOOST_PP_ENUM_PARAMS(BOOST_FUSION_TEST_MAX_N,Arg)>
    >
      : mpl::eval_if<
            mpl::empty<From>
          , mpl::identity<
                To<BOOST_PP_ENUM_PARAMS(BOOST_FUSION_TEST_MAX_N,Arg)>
            >
          , rebind_next_impl<
                From
              , To<BOOST_PP_ENUM_PARAMS(BOOST_FUSION_TEST_MAX_N,Arg)>
            >
        >
    {};
}

//TODO not necessary
template<typename From, typename To>
struct rebind;

template<
    typename From
  , template<BOOST_PP_ENUM_PARAMS(
        BOOST_FUSION_TEST_MAX_N, typename BOOST_PP_INTERCEPT)
    > class To
>
struct rebind<
    From
  , To<BOOST_PP_ENUM_PARAMS(
        BOOST_FUSION_TEST_MAX_N, fusion::void_ BOOST_PP_INTERCEPT)
    >
>
  : detail::rebind_impl<
        From
      , To<BOOST_PP_ENUM_PARAMS(
            BOOST_FUSION_TEST_MAX_N, fusion::void_ BOOST_PP_INTERCEPT)
        >
    >
{};
#else
#   include <boost/mpl/pop_back.hpp>

namespace detail
{
    template<typename From, template<typename...> class To, typename... Args>
    struct rebind_impl;

    template<typename From, template<typename...> class To, typename... Args>
    struct rebind_next_impl
      : rebind_impl<
            typename mpl::pop_back<From>::type
          , To
          , typename mpl::back<From>::type
          , Args...
        >
    {};

    template<typename From, template<typename...> class To, typename... Args>
    struct rebind_impl
      : mpl::eval_if<
            mpl::empty<From>
          , rebind_next_impl<From, To, Args...>
          , mpl::identity<To<Args...> >
        >
    {};
}

template<typename From, typename To>
struct rebind;

template<typename From, template<typename...> class To>
struct rebind<From, To<> >
  : detail::rebind_impl<From, To>
{};

#endif
