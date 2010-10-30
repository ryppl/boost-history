/*==============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/advance_c.hpp>
#include <boost/fusion/iterator/prior.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/next.hpp>
#ifndef BOOST_FUSION_NO_MEMBER_PTR
#   include <boost/fusion/functional/invocation/detail/member_object_pointer_helper.hpp>
#   include <boost/fusion/functional/invocation/detail/that_ptr.hpp>
#endif
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/internal/result_of.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/control/expr_iif.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/if.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_member_pointer.hpp>
#include <boost/type_traits/is_member_object_pointer.hpp>
#include <boost/type_traits/is_member_function_pointer.hpp>

namespace boost { namespace fusion { namespace detail
{
#ifndef BOOST_FUSION_RETURN_VOID
    template<typename F, typename... Args>
    struct BOOST_PP_CAT(
        BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME),
        _impl)
      : boost::result_of<typename get_func_base<F>::type(Args...)>
    {};
#endif

#ifdef BOOST_FUSION_NO_MEMBER_PTR
    template<typename Result, typename F, typename...Args>
    static Result
    BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_call_impl)(F&& f,Args&&...args)
    {
#   ifndef BOOST_FUSION_RETURN_VOID
        return
#   endif
            f(BOOST_FUSION_FORWARD(Args,args)...);
    }
#else
    template<typename Result, typename F, typename...Args>
    static typename disable_if<
        is_member_pointer<typename remove_reference<F>::type>
      , Result
    >::type
    BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_call_impl)(F&& f,Args&&...args)
    {
#   ifndef BOOST_FUSION_RETURN_VOID
        return
#   endif
            f(std::forward<Args>(args)...);
    }

    template<typename Result, typename F,  typename Instance,typename...Args>
    static typename enable_if<
        is_member_function_pointer<typename remove_reference<F>::type>
      , Result
    >::type
    BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_call_impl)(
        F&& f,
        Instance&& instance,
        Args&&... args)
    {
#   ifndef BOOST_FUSION_RETURN_VOID
        return
#   endif
            (that_ptr<typename preevaluate<F>::gen::class_type>::get(
                instance)->*f)(std::forward<Args>(args)...);
    }

#   ifndef BOOST_FUSION_RETURN_VOID
    template<typename Result, typename F,  typename Instance,typename...Args>
    static typename enable_if<
        is_member_object_pointer<typename remove_reference<F>::type>
      , Result
    >::type
    BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_call_impl)(
        F&& f,
        Instance&& instance,
        Args&&... args)
    {
        BOOST_FUSION_STATIC_ASSERT(!sizeof...(Args));

        return (that_ptr<
            typename get_object_pointer_result_type<
                Instance&&
              , typename remove_reference<F>::type
            >::cv_class_type&
        >::get(instance)->*f);
    }
#   endif
#endif

    namespace bidirectional_impl
    {
#ifndef BOOST_FUSION_RETURN_VOID

        template<
            int NumElementsLeft
          , typename F
          , typename It
          , typename...Args
        >
        struct BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME);

#   define BOOST_FUSION_RESULT_OF_UNROLLED_INVOKE_IMPL_IT_DEF(Z, N, MAX_N)      \
    typedef typename                                                            \
        result_of::prior<BOOST_PP_CAT(It,BOOST_PP_SUB(MAX_N, N)) const&>::type  \
    BOOST_PP_CAT(It,BOOST_PP_DEC(BOOST_PP_SUB(MAX_N, N)));

#   define BOOST_FUSION_RESULT_OF_UNROLLED_INVOKE_IMPL_SPECIALIZATION(N)        \
        template<typename F, typename BOOST_PP_CAT(It,N), typename... Args>     \
        struct BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME)<      \
            N                                                                   \
          , F                                                                   \
          , BOOST_PP_CAT(It,N)                                                  \
          , Args...                                                             \
        >

#   define BOOST_FUSION_RESULT_OF_UNROLLED_INVOKE_IMPL_DECLERATION(N)           \
        template<                                                               \
            int NumElementsLeft                                                 \
          , typename F                                                          \
          , typename BOOST_PP_CAT(It,N)                                         \
          , typename...Args                                                     \
        >                                                                       \
        struct BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME)

#   define BOOST_FUSION_RESULT_OF_UNROLLED_INVOKE_IMPL_NEXT()                   \
        BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME)<             \
            NumElementsLeft-BOOST_FUSION_UNROLLED_DEPTH                         \
          , F                                                                   \
          , typename result_of::prior<It0 const&>::type

#   define BOOST_FUSION_RESULT_OF_UNROLLED_INVOKE_IMPL_LAST()                   \
        detail::BOOST_PP_CAT(                                                   \
            BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME),         \
            _impl)                                                              \
        <                                                                       \
            F

#   define BOOST_FUSION_RESULT_OF_UNROLLED_INVOKE_IMPL(Z,N,_)                   \
        BOOST_PP_IIF(                                                           \
            BOOST_PP_EQUAL(N,BOOST_FUSION_UNROLLED_DEPTH),                      \
            BOOST_FUSION_RESULT_OF_UNROLLED_INVOKE_IMPL_DECLERATION,            \
            BOOST_FUSION_RESULT_OF_UNROLLED_INVOKE_IMPL_SPECIALIZATION)(N)      \
        {                                                                       \
            BOOST_PP_REPEAT(                                                    \
                N,                                                              \
                BOOST_FUSION_RESULT_OF_UNROLLED_INVOKE_IMPL_IT_DEF,             \
                N)                                                              \
                                                                                \
            typedef typename                                                    \
                BOOST_PP_IIF(                                                   \
                    BOOST_PP_EQUAL(N,BOOST_FUSION_UNROLLED_DEPTH),              \
                    BOOST_FUSION_RESULT_OF_UNROLLED_INVOKE_IMPL_NEXT,           \
                    BOOST_FUSION_RESULT_OF_UNROLLED_INVOKE_IMPL_LAST)()         \
                    BOOST_PP_COMMA_IF(N)                                        \
                    BOOST_PP_ENUM_BINARY_PARAMS(                                \
                        N,                                                      \
                        typename result_of::deref<It,                           \
                        >::type BOOST_PP_INTERCEPT)                             \
                  , Args...                                                     \
                >::type                                                         \
            type;                                                               \
        };

        BOOST_PP_REPEAT(
            BOOST_PP_INC(BOOST_FUSION_UNROLLED_DEPTH),
            BOOST_FUSION_RESULT_OF_UNROLLED_INVOKE_IMPL,
            _)

#   undef BOOST_FUSION_RESULT_OF_UNROLLED_INVOKE_IMPL
#   undef BOOST_FUSION_RESULT_OF_UNROLLED_INVOKE_IMPL_NEXT
#   undef BOOST_FUSION_RESULT_OF_UNROLLED_INVOKE_IMPL_LAST
#   undef BOOST_FUSION_RESULT_OF_UNROLLED_INVOKE_IMPL_DECLERATION
#   undef BOOST_FUSION_RESULT_OF_UNROLLED_INVOKE_IMPL_SPECIALIZATION
#   undef BOOST_FUSION_RESULT_OF_UNROLLED_INVOKE_IMPL_IT_DEF

#endif

        template<typename F, typename Result>
        struct BOOST_PP_CAT(unrolled_,BOOST_FUSION_INVOKE_NAME)
        {
#if defined(BOOST_NO_AUTO_DECLARATIONS) || defined(BOOST_FUSION_NO_RVALUE_REFERENCES)
#   define BOOST_FUSION_UNROLLED_INVOKE_IMPL_IT_DEF(Z, N, MAX_N)                \
    typedef typename                                                            \
        result_of::prior<BOOST_PP_CAT(It,BOOST_PP_SUB(MAX_N, N)) const&>::type  \
    BOOST_PP_CAT(It,BOOST_PP_DEC(BOOST_PP_SUB(MAX_N, N)));                      \
    BOOST_PP_CAT(It,BOOST_PP_DEC(BOOST_PP_SUB(MAX_N, N))) const                 \
        BOOST_PP_CAT(it,BOOST_PP_DEC(BOOST_PP_SUB(MAX_N, N)))=                  \
            fusion::prior(BOOST_PP_CAT(it,BOOST_PP_SUB(MAX_N, N)));
#else
#   define BOOST_FUSION_UNROLLED_INVOKE_IMPL_IT_DEF(Z, N, MAX_N)                \
    auto const BOOST_PP_CAT(it,BOOST_PP_DEC(BOOST_PP_SUB(MAX_N, N)))=           \
        fusion::prior(BOOST_PP_CAT(it,BOOST_PP_SUB(MAX_N, N)));
#endif

#define BOOST_FUSION_UNROLLED_INVOKE_IMPL_IT_ENUM(Z,N,_)                        \
    fusion::deref(BOOST_PP_CAT(it,N)),

#define BOOST_FUSION_UNROLLED_INVOKE_IMPL_NEXT()                                \
    call(                                                                       \
        mpl::int_<NumElementsLeft-BOOST_FUSION_UNROLLED_DEPTH>(),               \
        static_cast<F>(f),                                                      \
        it0

#define BOOST_FUSION_UNROLLED_INVOKE_IMPL_LAST()                                \
    BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_call_impl)<                          \
        Result                                                                  \
    >(static_cast<F>(f)

#define BOOST_FUSION_UNROLLED_INVOKE_IMPL(Z,N,_)                                \
            template<                                                           \
                BOOST_PP_EXPR_IIF(                                              \
                    BOOST_PP_EQUAL(N,BOOST_FUSION_UNROLLED_DEPTH),              \
                    int NumElementsLeft)                                        \
                BOOST_PP_COMMA_IF(                                              \
                    BOOST_PP_EQUAL(N,BOOST_FUSION_UNROLLED_DEPTH))              \
                typename BOOST_PP_CAT(It,N)                                     \
              , typename...Args                                                 \
            >                                                                   \
            static Result                                                       \
            call(                                                               \
                mpl::int_<BOOST_PP_IIF(                                         \
                    BOOST_PP_EQUAL(N,BOOST_FUSION_UNROLLED_DEPTH),              \
                    NumElementsLeft,                                            \
                    N)                                                          \
                >,                                                              \
                F f,                                                            \
                BOOST_PP_CAT(It,N) const& BOOST_PP_CAT(it,N),                   \
                Args&&... args)                                                 \
            {                                                                   \
                BOOST_PP_REPEAT(                                                \
                    N,                                                          \
                    BOOST_FUSION_UNROLLED_INVOKE_IMPL_IT_DEF,                   \
                    N)                                                          \
                                                                                \
                return BOOST_PP_IIF(                                            \
                    BOOST_PP_EQUAL(N,BOOST_FUSION_UNROLLED_DEPTH),              \
                    BOOST_FUSION_UNROLLED_INVOKE_IMPL_NEXT,                     \
                    BOOST_FUSION_UNROLLED_INVOKE_IMPL_LAST)(),                  \
                    BOOST_PP_REPEAT(                                            \
                        N, BOOST_FUSION_UNROLLED_INVOKE_IMPL_IT_ENUM, _)        \
                    std::forward<Args>(args)...);                               \
            }

            BOOST_PP_REPEAT(
                BOOST_PP_INC(BOOST_FUSION_UNROLLED_DEPTH),
                BOOST_FUSION_UNROLLED_INVOKE_IMPL,
                _)

#undef BOOST_FUSION_UNROLLED_INVOKE_IMPL
#undef BOOST_FUSION_UNROLLED_INVOKE_IMPL_NEXT
#undef BOOST_FUSION_UNROLLED_INVOKE_IMPL_LAST
#undef BOOST_FUSION_UNROLLED_INVOKE_IMPL_IT_ENUM
#undef BOOST_FUSION_UNROLLED_INVOKE_IMPL_IT_DEF
        };
    }

    namespace forward_impl
    {
#ifndef BOOST_FUSION_RETURN_VOID
        template<int NumArgsLeft, typename F, typename Begin, typename... Args>
        struct BOOST_PP_CAT(result_of_,BOOST_FUSION_INVOKE_NAME)
          : BOOST_PP_CAT(result_of_,BOOST_FUSION_INVOKE_NAME)<
                NumArgsLeft-1
              , F
              , Begin
              , typename result_of::deref<
                    typename result_of::advance_c<Begin, NumArgsLeft-1>::type
                >::type
              , Args...
            >
        {};

        template<typename F, typename Begin, typename... Args>
        struct BOOST_PP_CAT(result_of_,BOOST_FUSION_INVOKE_NAME)<
            0
          , F
          , Begin
          , Args...
        >
          : BOOST_PP_CAT(
                BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME),
                _impl)<
                F
              , Args...
            >
        {};
#endif

        template<typename F,typename Result>
        struct BOOST_FUSION_INVOKE_NAME
        {
            template<typename Begin, typename... Args>
            static Result
            call(mpl::int_<0>, F f,Begin const&, Args&&... args)
            {
                return BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_call_impl)<
                    Result
                >(static_cast<F>(f),std::forward<Args>(args)...);
            }

            template<int NumArgsLeft,typename Begin, typename... Args>
            static Result
            call(mpl::int_<NumArgsLeft>, F f, Begin const& begin,Args&&... args)
            {
                return call(
                    mpl::int_<NumArgsLeft-1>(),
                    static_cast<F>(f),
                    begin,
                    fusion::deref(fusion::advance_c<NumArgsLeft-1>(begin)),
                    std::forward<Args>(args)...);
            }
        };
    }

    template<typename F, typename SeqRef>
    struct BOOST_FUSION_INVOKE_NAME
    {
#ifdef BOOST_FUSION_RETURN_VOID
        typedef void type;
#else
        typedef
            mpl::eval_if_c<
                traits::is_bidirectional<SeqRef>::value
              , bidirectional_impl::
                    BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME)<
                    result_of::size<SeqRef>::value
                  , F
                  , typename result_of::end<SeqRef>::type
                >
              , forward_impl::
                    BOOST_PP_CAT(result_of_,BOOST_FUSION_INVOKE_NAME)<
                    result_of::size<SeqRef>::value
                  , F
                  , typename result_of::begin<SeqRef>::type
                >
            >
        impl_type;
        typedef typename detail::remove_reference<F>::type f;
        typedef preevaluate<F> preevaluater;

        typedef typename
            mpl::eval_if_c<
                preevaluater::is_preevaluable::value
              , preevaluater
#   ifndef BOOST_FUSION_NO_MEMBER_PTR
              , mpl::eval_if<
                    is_member_object_pointer<f>
                  , get_object_pointer_result_type_seq<f, SeqRef>
#   endif
                  , impl_type
#   ifndef BOOST_FUSION_NO_MEMBER_PTR
                >
#   endif
            >::type
        type;
#endif

        static type
        call_impl(mpl::false_ /*is_bidirectional*/,
                    F f,
                    SeqRef seq)
        {
            return forward_impl::BOOST_FUSION_INVOKE_NAME<
                F
              , type
           >::call(mpl::int_<result_of::size<SeqRef>::value>(),
                   static_cast<F>(f),
                   fusion::begin(static_cast<SeqRef>(seq)));
        }

        static type
        call_impl(mpl::true_ /*is_bidirectional*/,
                    F f,
                    SeqRef seq)
        {
            return bidirectional_impl::BOOST_PP_CAT(unrolled_,
                BOOST_FUSION_INVOKE_NAME)<
                F
              , type
            >::call(mpl::int_<result_of::size<SeqRef>::value>(),
                    static_cast<F>(f),
                    fusion::end(static_cast<SeqRef>(seq)));
        }

        static type
        call(F f, SeqRef seq)
        {
            return call_impl(
                traits::is_bidirectional<SeqRef>(),
                static_cast<F>(f),
                static_cast<SeqRef>(seq));
        }
    };
}}}

