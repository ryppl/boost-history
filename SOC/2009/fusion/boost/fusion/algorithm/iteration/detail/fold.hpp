/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_ITERATION_DETAIL_FOLD_HPP
#define BOOST_FUSION_ALGORITHM_ITERATION_DETAIL_FOLD_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/sequence/intrinsic/empty.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/iterator/prior.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/support/internal/result_of.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
#   include <boost/type_traits/add_const.hpp>
#endif

#endif

#ifdef BOOST_FUSION_REVERSE_FOLD
#   ifdef BOOST_FUSION_ITER_FOLD
#       define BOOST_FUSION_FOLD_NAME reverse_iter_fold
#   else
#       define BOOST_FUSION_FOLD_NAME reverse_fold
#   endif

#   define BOOST_FUSION_FOLD_IMPL_FIRST_IT_FUNCTION end
#   define BOOST_FUSION_FOLD_IMPL_NEXT_IT_FUNCTION prior
#   define BOOST_FUSION_FOLD_IMPL_FIRST_IT_META_TRANSFORM(IT)                   \
        typename fusion::result_of::prior<IT>::type
#   define BOOST_FUSION_FOLD_IMPL_FIRST_IT_TRANSFORM(IT) fusion::prior(IT)
#else
#   ifdef BOOST_FUSION_ITER_FOLD
#       define BOOST_FUSION_FOLD_NAME iter_fold
#   else
#       define BOOST_FUSION_FOLD_NAME fold
#   endif

#   define BOOST_FUSION_FOLD_IMPL_FIRST_IT_FUNCTION begin
#   define BOOST_FUSION_FOLD_IMPL_NEXT_IT_FUNCTION next
#   define BOOST_FUSION_FOLD_IMPL_FIRST_IT_META_TRANSFORM(IT) IT
#   define BOOST_FUSION_FOLD_IMPL_FIRST_IT_TRANSFORM(IT) IT
#endif
#ifdef BOOST_FUSION_ITER_FOLD
#   define BOOST_FUSION_FOLD_IMPL_INVOKE_IT_META_TRANSFORM(IT) IT
#   define BOOST_FUSION_FOLD_IMPL_INVOKE_IT_TRANSFORM(IT) IT
#else
#   define BOOST_FUSION_FOLD_IMPL_INVOKE_IT_META_TRANSFORM(IT)                  \
        typename fusion::result_of::deref<IT>::type
#   define BOOST_FUSION_FOLD_IMPL_INVOKE_IT_TRANSFORM(IT) fusion::deref(IT)
#endif

namespace boost { namespace fusion
{
    namespace detail
    {
        template<typename State, typename It, typename F>
        struct BOOST_PP_CAT(BOOST_FUSION_FOLD_NAME, _result_of_helper)
          : boost::result_of<
                F(
#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
                typename add_lref<typename add_const<State>::type>::type,
#else
                typename remove_reference<State>::type&,
#endif
                BOOST_FUSION_FOLD_IMPL_INVOKE_IT_META_TRANSFORM(It))
            >
        {};

        template<typename State0, typename It0, typename F, int N>
        struct BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_FOLD_NAME);

#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
#   define BOOST_FUSION_CONST_IF_NO_RVALUE_REFERENCES const
#else
#   define BOOST_FUSION_CONST_IF_NO_RVALUE_REFERENCES
#endif

#define BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL_IT_DEF(Z, N, _)               \
        typedef typename                                                        \
            result_of::BOOST_FUSION_FOLD_IMPL_NEXT_IT_FUNCTION<                 \
                BOOST_PP_CAT(It,N) const&                                       \
            >::type                                                             \
        BOOST_PP_CAT(It,BOOST_PP_INC(N));

#define BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL_STATE_DEF(Z, N, _)            \
        typedef typename                                                        \
            BOOST_PP_CAT(BOOST_FUSION_FOLD_NAME, _result_of_helper)<            \
                BOOST_PP_CAT(State,N) BOOST_FUSION_CONST_IF_NO_RVALUE_REFERENCES\
              , BOOST_PP_CAT(It,N) const&                                       \
              , F                                                               \
            >::type                                                             \
        BOOST_PP_CAT(State,BOOST_PP_INC(N));                                    \

#define BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL_NEXT(N_)                      \
        typename BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_FOLD_NAME)<      \
            BOOST_PP_CAT(State,N_)                                              \
          , typename result_of::BOOST_FUSION_FOLD_IMPL_NEXT_IT_FUNCTION<        \
                BOOST_PP_CAT(It,BOOST_PP_DEC(N_)) const&                        \
            >::type                                                             \
          , F                                                                   \
          , N-BOOST_FUSION_UNROLLED_DEPTH                                       \
        >::type

#define BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL_LAST(N) BOOST_PP_CAT(State,N)

#define BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL_DECLERATION(N_)               \
        template<typename State0, typename It0, typename F, int N>              \
        struct BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_FOLD_NAME)

#define BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL_SPECIALIZATION(N)             \
        template<typename State0, typename It0, typename F>                     \
        struct BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_FOLD_NAME)<        \
            State0                                                              \
          , It0                                                                 \
          , F                                                                   \
          , N                                                                   \
        >

#define BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL(Z,N,_)                        \
        BOOST_PP_IIF(                                                           \
            BOOST_PP_EQUAL(N,BOOST_FUSION_UNROLLED_DEPTH),                      \
            BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL_DECLERATION,              \
            BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL_SPECIALIZATION)(N)        \
        {                                                                       \
            BOOST_PP_REPEAT(                                                    \
                BOOST_PP_DEC(N),                                                \
                BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL_IT_DEF,               \
                _)                                                              \
            BOOST_PP_REPEAT(                                                    \
                N,                                                              \
                BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL_STATE_DEF,            \
                _)                                                              \
                                                                                \
            typedef                                                             \
                BOOST_PP_IIF(                                                   \
                    BOOST_PP_EQUAL(N,BOOST_FUSION_UNROLLED_DEPTH),              \
                    BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL_NEXT,             \
                    BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL_LAST)(N)          \
            type;                                                               \
        };

#ifdef BOOST_MSVC
#   pragma warning(push)
//warning C4181: qualifier applied to reference type; ignored
#   pragma warning(disable: 4181)
#endif

        BOOST_PP_REPEAT_FROM_TO(
            1,
            BOOST_PP_INC(BOOST_FUSION_UNROLLED_DEPTH),
            BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL,
            _)

#ifdef BOOST_MSVC
#   pragma warning(pop)
#endif

#undef BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL
#undef BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL_DECLERATION
#undef BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL_SPECIALIZATION
#undef BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL_NEXT
#undef BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL_LAST
#undef BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL_STATE_DEF
#undef BOOST_FUSION_RESULT_OF_UNROLLED_FOLD_IMPL_IT_DEF

        template<typename State0, typename It0, typename F>
        struct BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_FOLD_NAME)<
            State0
          , It0
          , F
          , 0
        >
        {
            typedef State0 type;
        };

        template<typename FFuncBase, typename Result,int N>
        struct BOOST_PP_CAT(unrolled_,BOOST_FUSION_FOLD_NAME);

#if defined(BOOST_NO_AUTO_DECLARATIONS) || defined(BOOST_FUSION_NO_RVALUE_REFERENCES)
#   define BOOST_FUSION_UNROLLED_FOLD_IMPL_LOCAL_INIT(Z,N,_)                    \
        typedef typename                                                        \
            result_of::BOOST_FUSION_FOLD_IMPL_NEXT_IT_FUNCTION<                 \
                BOOST_PP_CAT(It,N) const&                                       \
            >::type                                                             \
        BOOST_PP_CAT(It,BOOST_PP_INC(N));                                       \
        BOOST_PP_CAT(It,BOOST_PP_INC(N)) const BOOST_PP_CAT(it,BOOST_PP_INC(N))(\
            fusion::BOOST_FUSION_FOLD_IMPL_NEXT_IT_FUNCTION(                    \
                BOOST_PP_CAT(it,N)));                                           \
                                                                                \
        typedef typename BOOST_PP_CAT(BOOST_FUSION_FOLD_NAME,_result_of_helper)<\
            BOOST_PP_CAT(State,N) BOOST_FUSION_CONST_IF_NO_RVALUE_REFERENCES    \
          , BOOST_PP_CAT(It,N) const&                                           \
          , FFuncBase                                                           \
        >::type BOOST_PP_CAT(State,BOOST_PP_INC(N));                            \
        BOOST_PP_CAT(State,BOOST_PP_INC(N))                                     \
            BOOST_FUSION_CONST_IF_NO_RVALUE_REFERENCES                          \
            BOOST_PP_CAT(state,BOOST_PP_INC(N))(                                \
                f(                                                              \
                    BOOST_PP_CAT(state,N),                                      \
                    BOOST_FUSION_FOLD_IMPL_INVOKE_IT_TRANSFORM(                 \
                        BOOST_PP_CAT(it,N)))                                    \
                );
#else
#   define BOOST_FUSION_UNROLLED_FOLD_IMPL_LOCAL_INIT(Z,N,_)                    \
        auto const BOOST_PP_CAT(it,BOOST_PP_INC(N))=                            \
            fusion::BOOST_FUSION_FOLD_IMPL_NEXT_IT_FUNCTION(BOOST_PP_CAT(it,N));\
        auto&& BOOST_PP_CAT(state,BOOST_PP_INC(N))=f(                           \
            BOOST_PP_CAT(state,N),                                              \
            BOOST_FUSION_FOLD_IMPL_INVOKE_IT_TRANSFORM(BOOST_PP_CAT(it,N)));
#endif

#define BOOST_FUSION_UNROLLED_FOLD_IMPL_DECLERATION(N_)                         \
        template<typename FFuncBase, typename Result,int N>                     \
        struct BOOST_PP_CAT(unrolled_,BOOST_FUSION_FOLD_NAME)

#define BOOST_FUSION_UNROLLED_FOLD_IMPL_SPECIALIZATION(N)                       \
        template<typename FFuncBase, typename Result>                           \
        struct BOOST_PP_CAT(unrolled_,BOOST_FUSION_FOLD_NAME)<                  \
            FFuncBase                                                           \
          , Result                                                              \
          , N                                                                   \
        >

#define BOOST_FUSION_UNROLLED_FOLD_IMPL_NEXT(N_)                                \
        BOOST_PP_CAT(unrolled_,BOOST_FUSION_FOLD_NAME)<                         \
            FFuncBase                                                           \
          , Result                                                              \
          , N-BOOST_FUSION_UNROLLED_DEPTH                                       \
        >::call(f(                                                              \
                BOOST_PP_CAT(state,BOOST_PP_DEC(N_)),                           \
                BOOST_FUSION_FOLD_IMPL_INVOKE_IT_TRANSFORM(                     \
                    BOOST_PP_CAT(it,BOOST_PP_DEC(N_)))),                        \
            fusion::BOOST_FUSION_FOLD_IMPL_NEXT_IT_FUNCTION(                    \
                BOOST_PP_CAT(it,BOOST_PP_DEC(N_))),                             \
            f)

#define BOOST_FUSION_UNROLLED_FOLD_IMPL_RETURN(N)                               \
        f(BOOST_PP_CAT(state,BOOST_PP_DEC(N)),                                  \
            BOOST_FUSION_FOLD_IMPL_INVOKE_IT_TRANSFORM(                         \
                BOOST_PP_CAT(it,BOOST_PP_DEC(N))))

#define BOOST_FUSION_UNROLLED_FOLD_IMPL(Z,N,_)                                  \
        BOOST_PP_IIF(                                                           \
            BOOST_PP_EQUAL(N,BOOST_FUSION_UNROLLED_DEPTH),                      \
            BOOST_FUSION_UNROLLED_FOLD_IMPL_DECLERATION,                        \
            BOOST_FUSION_UNROLLED_FOLD_IMPL_SPECIALIZATION)(N)                  \
        {                                                                       \
            template<typename State0, typename It0, typename F>                 \
            static Result                                                       \
            call(BOOST_FUSION_R_ELSE_CLREF(State0) state0,                      \
                It0 const& it0,                                                 \
                BOOST_FUSION_RREF_ELSE_OBJ(F) f)                                \
            {                                                                   \
                BOOST_PP_REPEAT(                                                \
                    BOOST_PP_DEC(N),                                            \
                    BOOST_FUSION_UNROLLED_FOLD_IMPL_LOCAL_INIT,                 \
                    _)                                                          \
                                                                                \
                return BOOST_PP_IIF(                                            \
                    BOOST_PP_EQUAL(N,BOOST_FUSION_UNROLLED_DEPTH),              \
                    BOOST_FUSION_UNROLLED_FOLD_IMPL_NEXT,                       \
                    BOOST_FUSION_UNROLLED_FOLD_IMPL_RETURN)(N);                 \
            }                                                                   \
        };

        BOOST_PP_REPEAT_FROM_TO(
            1,
            BOOST_PP_INC(BOOST_FUSION_UNROLLED_DEPTH),
            BOOST_FUSION_UNROLLED_FOLD_IMPL,
            _)

#undef BOOST_FUSION_UNROLLED_FOLD_IMPL
#undef BOOST_FUSION_UNROLLED_FOLD_IMPL_NEXT
#undef BOOST_FUSION_UNROLLED_FOLD_IMPL_RETURN
#undef BOOST_FUSION_UNROLLED_FOLD_IMPL_SPECIALIZATION
#undef BOOST_FUSION_UNROLLED_FOLD_IMPL_DECLERATION
#undef BOOST_FUSION_UNROLLED_FOLD_IMPL_LOCAL_INIT

#undef BOOST_FUSION_CONST_IF_NO_RVALUE_REFERENCES

        template<typename FFuncBase, typename Result>
        struct BOOST_PP_CAT(unrolled_,BOOST_FUSION_FOLD_NAME)<
            FFuncBase
          , Result
          , 0
        >
        {
            template<typename State0, typename It0, typename F>
            static Result
            call(BOOST_FUSION_R_ELSE_CLREF(State0) state0,
                It0 const&,
                BOOST_FUSION_RREF_ELSE_OBJ(F))
            {
                return static_cast<Result>(state0);
            }
        };

        template<typename StateRef, typename It0, typename F, int SeqSize>
        struct BOOST_PP_CAT(result_of_first_unrolled,BOOST_FUSION_FOLD_NAME)
          : BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_FOLD_NAME)<
                StateRef
              , It0
              , typename get_func_base<F>::type
              , SeqSize
            >
        {};

        template<int SeqSize, typename StateRef, typename It0, typename F>
        struct BOOST_PP_CAT(BOOST_FUSION_FOLD_NAME,_impl)
        {
            typedef preevaluate<F> preevaluater;

            typedef typename
                mpl::eval_if_c<
                    preevaluater::is_preevaluable::value
                  , preevaluater
                  , BOOST_PP_CAT(
                        result_of_first_unrolled,BOOST_FUSION_FOLD_NAME)<
                        StateRef
                      , BOOST_FUSION_FOLD_IMPL_FIRST_IT_META_TRANSFORM(It0)
                      , F
                      , SeqSize
                    >
                >::type
            type;

            static type
            call(StateRef state, It0 const& it0, F f)
            {
                return BOOST_PP_CAT(unrolled_,BOOST_FUSION_FOLD_NAME)<
                    typename get_func_base<F>::type
                  , type
                  , SeqSize
                >::call(
                    BOOST_FUSION_FORWARD(StateRef,state),
                    BOOST_FUSION_FOLD_IMPL_FIRST_IT_TRANSFORM(it0),
                    BOOST_FUSION_FORWARD(F,f));
            }
        };

        template<typename StateRef, typename It0, typename F>
        struct BOOST_PP_CAT(BOOST_FUSION_FOLD_NAME,_impl)<0,StateRef,It0,F>
        {
            typedef StateRef type;

            static StateRef
            call(StateRef state, It0 const&, F)
            {
                return static_cast<StateRef>(state);
            }
        };
    }

    namespace result_of
    {
        template<typename Seq, typename State, typename F>
        struct BOOST_FUSION_FOLD_NAME
          : detail::BOOST_PP_CAT(BOOST_FUSION_FOLD_NAME,_impl)<
                size<Seq>::value
              , typename detail::add_lref<
#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
                    typename add_const<
#endif
                        State
#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
                    >::type
#endif
                >::type
              , typename BOOST_FUSION_FOLD_IMPL_FIRST_IT_FUNCTION<Seq>::type
              , F
            >
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>))
#ifdef BOOST_FUSION_REVERSE_FOLD
            BOOST_FUSION_MPL_ASSERT((traits::is_bidirectional<Seq>))
#else
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>))
#endif
        };
    }

    template<typename Seq, typename State, typename F>
    inline typename result_of::BOOST_FUSION_FOLD_NAME<
        BOOST_FUSION_R_ELSE_CLREF(Seq)
      , BOOST_FUSION_R_ELSE_CLREF(State)
      , BOOST_FUSION_RREF_ELSE_OBJ(F)
    >::type
    BOOST_FUSION_FOLD_NAME(BOOST_FUSION_R_ELSE_CLREF(Seq) seq,
        BOOST_FUSION_R_ELSE_CLREF(State) state,
        BOOST_FUSION_RREF_ELSE_OBJ(F) f)
    {
        return result_of::BOOST_FUSION_FOLD_NAME<
            BOOST_FUSION_R_ELSE_CLREF(Seq)
          , BOOST_FUSION_R_ELSE_CLREF(State)
          , BOOST_FUSION_RREF_ELSE_OBJ(F)
        >::call(
            BOOST_FUSION_FORWARD(State,state),
            fusion::BOOST_FUSION_FOLD_IMPL_FIRST_IT_FUNCTION(
                BOOST_FUSION_FORWARD(Seq,seq)),
            BOOST_FUSION_FORWARD(F,f));
    }

#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
    template<typename Seq, typename State, typename F>
    inline typename result_of::BOOST_FUSION_FOLD_NAME<
        Seq&
      , State const&
      , F
    >::type
    BOOST_FUSION_FOLD_NAME(Seq& seq,State const& state,F f)
    {
        return result_of::BOOST_FUSION_FOLD_NAME<Seq&,State const&,F>::call(
            state,
            fusion::BOOST_FUSION_FOLD_IMPL_FIRST_IT_FUNCTION(seq),
            f);
    }
#endif
}}

#undef BOOST_FUSION_FOLD_NAME
#undef BOOST_FUSION_FOLD_IMPL_FIRST_IT_FUNCTION
#undef BOOST_FUSION_FOLD_IMPL_NEXT_IT_FUNCTION
#undef BOOST_FUSION_FOLD_IMPL_FIRST_IT_META_TRANSFORM
#undef BOOST_FUSION_FOLD_IMPL_FIRST_IT_TRANSFORM
#undef BOOST_FUSION_FOLD_IMPL_INVOKE_IT_META_TRANSFORM
#undef BOOST_FUSION_FOLD_IMPL_INVOKE_IT_TRANSFORM
