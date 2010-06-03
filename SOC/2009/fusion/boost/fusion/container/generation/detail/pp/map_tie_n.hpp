/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#define BOOST_FUSION_N BOOST_PP_ITERATION()
#if BOOST_FUSION_N
#   define BOOST_FUSION_COMMA ,
#else
#   define BOOST_FUSION_COMMA
#endif

    namespace result_of
    {
        template<
            BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, typename K)
            BOOST_FUSION_COMMA
            BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, typename T)
        >
        struct map_tie
#if BOOST_FUSION_N!=FUSION_MAX_MAP_SIZE
        <
            BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, K)
            BOOST_FUSION_COMMA
            BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, T)
        >
#endif
        {
#define BOOST_FUSION_MAP_ELEMENT(Z,N,_)\
            pair<\
                BOOST_PP_CAT(K,N)\
              , typename detail::deduce_ref<BOOST_PP_CAT(T,N)>::type\
            >

            typedef
                map<BOOST_PP_ENUM(BOOST_FUSION_N, BOOST_FUSION_MAP_ELEMENT, _)>
            type;

#undef BOOST_FUSION_MAP_ELEMENT
        };
    }

#if BOOST_FUSION_N
    template<
        BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, typename K)
      , BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, typename A)
    >
#endif
    inline
#if BOOST_FUSION_N
        typename
#endif
        result_of::map_tie<
            BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, K)
            BOOST_FUSION_COMMA
            EXPAND_TEMPLATE_ARGUMENTS_A_R_ELSE_LREF(BOOST_FUSION_N)
        >::type
    map_tie(EXPAND_TEMPLATE_ARGUMENTS_PARAMETERS_A_R_ELSE_LREF(BOOST_FUSION_N))
    {
        return
#if BOOST_FUSION_N
            typename
#endif
            result_of::map_tie<
                BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, K)
                BOOST_FUSION_COMMA
                EXPAND_TEMPLATE_ARGUMENTS_A_R_ELSE_LREF(BOOST_FUSION_N)
            >::type(
#ifdef BOOST_NO_RVALUE_REFERENCES
                EXPAND_PARAMETERS_A(BOOST_FUSION_N)
#else

#   define BOOST_FUSION_MAP_TIE_FORWARD_ARGS(Z,N,__)\
                static_cast<\
                    typename detail::deduce_ref<BOOST_PP_CAT(A,N)&&>::type\
                >(BOOST_PP_CAT(_,N))

                BOOST_PP_ENUM(
                    BOOST_FUSION_N,BOOST_FUSION_MAP_TIE_FORWARD_ARGS,_)

#   undef BOOST_FUSION_MAP_TIE_FORWARD_ARGS

#endif
            );
    }

#undef BOOST_FUSION_COMMA
#undef BOOST_FUSION_N
