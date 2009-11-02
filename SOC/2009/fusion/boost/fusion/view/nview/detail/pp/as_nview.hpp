/*=============================================================================
    Copyright (c) 2009 Hartmut Kaiser
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#define BOOST_FUSION_N BOOST_PP_ITERATION()

    namespace result_of
    {
        template<
            typename Seq
#if BOOST_FUSION_N
          , BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, int I)
#endif
        >
        struct as_nview
#if BOOST_FUSION_N!=FUSION_MAX_VECTOR_SIZE
        <
            Seq
#   if BOOST_FUSION_N
          , BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, I)
#   endif
        >
#endif
        {
            typedef
                nview<
                    Seq
                  , mpl::BOOST_PP_CAT(BOOST_PP_CAT(vector,BOOST_FUSION_N),_c)<
                        int
#if BOOST_FUSION_N
                      , BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, I)
#endif
                    >
                >
            type;
        };
    }

    template<
#if BOOST_FUSION_N
        BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, int I),
#endif
        typename Seq
    >
    typename result_of::as_nview<
        BOOST_FUSION_R_ELSE_CLREF(Seq)
#if BOOST_FUSION_N
      , BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, I)
#endif
    >::type
    as_nview(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return typename result_of::as_nview<
                BOOST_FUSION_R_ELSE_CLREF(Seq)
#if BOOST_FUSION_N
              , BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, I)
#endif
            >::type(BOOST_FUSION_FORWARD(Seq,seq));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template<
#   if BOOST_FUSION_N
        BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, int I),
#   endif
        typename Seq
    >
//cschmidt: see https://svn.boost.org/trac/boost/ticket/3305
#   if defined(BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS) || BOOST_WORKAROUND(__GNUC__,<4)
    typename lazy_disable_if<
        is_const<Seq>
      , result_of::as_nview<
            Seq&
#   if BOOST_FUSION_N
          , BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, I)
#   endif
        >
    >::type
#   else
    typename result_of::as_nview<
        Seq&
#       if BOOST_FUSION_N
      , BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, I)
#       endif
    >::type
#   endif
    as_nview(Seq& seq)
    {
        return typename result_of::as_nview<
                Seq&
#   if BOOST_FUSION_N
              , BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, I)
#   endif
            >::type(seq);
    }
#endif

#undef BOOST_FUSION_N
