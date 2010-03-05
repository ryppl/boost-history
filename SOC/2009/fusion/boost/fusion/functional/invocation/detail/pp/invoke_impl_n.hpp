/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#define BOOST_FUSION_N BOOST_PP_ITERATION()

namespace random_access_impl
{
#ifndef BOOST_FUSION_RETURN_VOID
#   define BOOST_FUSION_RESULT_OF_AT_SEQ_N(Z,N,_)\
    typename result_of::at_c<SeqRef,N>::type

    template<typename F, typename SeqRef>
    struct BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME)<
        BOOST_FUSION_N
      , F
      , SeqRef>
    {
        typedef typename
            boost::result_of<
                typename get_func_base<F>::type(BOOST_PP_ENUM_1(
                    BOOST_FUSION_N,BOOST_FUSION_RESULT_OF_AT_SEQ_N,_))
            >::type
        type;
    };

#   undef BOOST_FUSION_RESULT_OF_AT_SEQ_N
#endif

#define BOOST_FUSION_SEQ_AT_N(Z,N,_) fusion::at_c<N>(static_cast<SeqRef>(seq))

    template<typename F, typename SeqRef, typename Result>
    struct BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_impl)<
        function_object
      , BOOST_FUSION_N
      , F
      , SeqRef
      , Result
    >
    {
        static Result
        call(F f, SeqRef seq)
        {
#ifndef BOOST_FUSION_RETURN_VOID
            return
#endif
                f(BOOST_PP_ENUM_1(BOOST_FUSION_N,BOOST_FUSION_SEQ_AT_N,_));
        }
    };

#if !defined(BOOST_FUSION_NO_MEMBER_PTR) && BOOST_FUSION_N>=1
    template<typename F, typename SeqRef, typename Result>
    struct BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_impl)<
        member_function_ptr
      , BOOST_FUSION_N
      , F
      , SeqRef
      , Result
    >
    {
        static Result
        call(F f, SeqRef seq)
        {
#   ifndef BOOST_FUSION_RETURN_VOID
            return
#   endif
                (that_ptr<typename preevaluate<F>::gen::class_type>::get(
                    fusion::front(seq))->*f)(BOOST_PP_ENUM_SHIFTED_1(
                        BOOST_FUSION_N,BOOST_FUSION_SEQ_AT_N,_));
        }
    };

#   if BOOST_FUSION_N==1
    template<typename F, typename SeqRef, typename Result>
    struct BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_impl)<
        member_object_ptr
      , 1
      , F
      , SeqRef
      , Result
    >
    {
        static Result
        call(F f, SeqRef seq)
        {
            typedef typename result_of::front<SeqRef>::type Instance;

            return (that_ptr<
                        typename get_object_pointer_result_type<
                            Instance
                          , typename remove_reference<F>::type
                        >::cv_class_type&
                    >::get(fusion::front(seq))->*f);
        }
    };
#   endif
#endif

#undef BOOST_FUSION_SEQ_AT_N
}

namespace forward_impl
{
#ifndef BOOST_FUSION_RETURN_VOID
    template<typename F, typename SeqRef>
    struct BOOST_PP_CAT(result_of_,BOOST_FUSION_INVOKE_NAME)<
        BOOST_FUSION_N
      , F
      , SeqRef>
    {
#   if BOOST_FUSION_N
        typedef typename result_of::begin<SeqRef>::type It0;

#       if BOOST_FUSION_N > 1
#       define BOOST_FUSION_IT_N(Z,N,_)\
        typedef typename\
            result_of::next<BOOST_PP_CAT(It, BOOST_PP_DEC(N))>::type\
        BOOST_PP_CAT(It,N);\

        BOOST_PP_REPEAT_FROM_TO_1(1, BOOST_FUSION_N, BOOST_FUSION_IT_N, _)

#       undef BOOST_FUSION_IT_N
#       endif
#   endif

        typedef typename
            boost::result_of<
                typename get_func_base<F>::type(
                BOOST_PP_ENUM_BINARY_PARAMS_Z(
                    1,
                    BOOST_FUSION_N,
                    typename result_of::deref<It,
                    >::type BOOST_PP_INTERCEPT)
                )
            >::type
        type;
    };
#endif


#if BOOST_FUSION_N
#   define BOOST_FUSION_IT_N(Z,N,_)\
        typedef typename\
            result_of::next<BOOST_PP_CAT(It, BOOST_PP_DEC(N))>::type\
        BOOST_PP_CAT(It,N);\
        BOOST_PP_CAT(It,N) BOOST_PP_CAT(it,N)=\
            fusion::next(BOOST_PP_CAT(it, BOOST_PP_DEC(N)));

#   define BOOST_FUSION_DEFINE_ITS()\
        typedef typename result_of::begin<SeqRef>::type It0;\
        It0 it0=fusion::begin(seq);\
        \
        BOOST_PP_REPEAT_FROM_TO(1, BOOST_FUSION_N, BOOST_FUSION_IT_N, _)

#   define BOOST_FUSION_IT_DEREF_N(Z,N,_)\
        fusion::deref(BOOST_PP_CAT(it,N))
#else
#   define BOOST_FUSION_DEFINE_ITS()
#endif

    template<typename F, typename SeqRef, typename Result>
    struct BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_impl)<
        function_object
      , BOOST_FUSION_N
      , F
      , SeqRef
      , Result
    >
    {
        static Result
        call(F f, SeqRef seq)
        {
            BOOST_FUSION_DEFINE_ITS()

#ifndef BOOST_FUSION_RETURN_VOID
            return
#endif
                f(BOOST_PP_ENUM_1(BOOST_FUSION_N,BOOST_FUSION_IT_DEREF_N,_));
        }
    };

#if !defined(BOOST_FUSION_NO_MEMBER_PTR) && BOOST_FUSION_N>=1
    template<typename F, typename SeqRef, typename Result>
    struct BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_impl)<
        member_function_ptr
      , BOOST_FUSION_N
      , F
      , SeqRef
      , Result
    >
    {
        static Result
        call(F f, SeqRef seq)
        {
            BOOST_FUSION_DEFINE_ITS()

#   ifndef BOOST_FUSION_RETURN_VOID
            return
#   endif
                (that_ptr<typename preevaluate<F>::gen::class_type>::get(
                    fusion::deref(it0))->*f)(BOOST_PP_ENUM_SHIFTED_1(
                        BOOST_FUSION_N,BOOST_FUSION_IT_DEREF_N,_));
        }
    };

#   if BOOST_FUSION_N==1
    template<typename F, typename SeqRef, typename Result>
    struct BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_impl)<
        member_object_ptr
      , 1
      , F
      , SeqRef
      , Result
    >
    {
        static Result
        call(F f, SeqRef seq)
        {
            typedef typename result_of::front<SeqRef>::type Instance;

            return (that_ptr<
                        typename get_object_pointer_result_type<
                            Instance
                          , typename remove_reference<F>::type
                        >::cv_class_type&
                    >::get(fusion::front(seq))->*f);
        }
    };
#   endif
#endif

#if BOOST_FUSION_N
#   undef BOOST_FUSION_IT_DEREF_N
#   undef BOOST_FUSION_IT_N
#endif
#undef BOOST_FUSION_DEFINE_ITS
}

#undef BOOST_FUSION_N

