// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PP_IS_ITERATING
#ifndef BOOST_FUSION_CONTAINER_GENERATION_DETAIL_PP_MAKE_MAP_HPP
#define BOOST_FUSION_CONTAINER_GENERATION_DETAIL_PP_MAKE_MAP_HPP

#include <boost/fusion/container/map/map.hpp>
#include <boost/fusion/support/internal/as_fusion_element.hpp>
#include <boost/fusion/support/pair.hpp>

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/empty.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template<
            BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(
                    FUSION_MAX_MAP_SIZE,
                    typename K,
                    void_)
          , BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(
                    FUSION_MAX_MAP_SIZE,
                    typename T,
                    void_)
        >
        struct make_map;
    }
    
#define BOOST_PP_FILENAME_1 <boost/fusion/container/generation/detail/pp/make_map.hpp>
#define BOOST_PP_ITERATION_LIMITS (0, FUSION_MAX_MAP_SIZE)
#include BOOST_PP_ITERATE()

}}

#endif
#else

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
        struct make_map
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
              , typename detail::as_fusion_element<BOOST_PP_CAT(T,N)>::type\
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
        result_of::make_map<
            BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, K)
            BOOST_FUSION_COMMA
            EXPAND_TEMPLATE_ARGUMENTS_A_R_ELSE_CLREF(BOOST_FUSION_N)
        >::type
    make_map(
        EXPAND_TEMPLATE_ARGUMENTS_PARAMETERS_A_R_ELSE_CLREF(BOOST_FUSION_N))
    {
        return
#if BOOST_FUSION_N
            typename
#endif
            result_of::make_map<
                BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, K)
                BOOST_FUSION_COMMA
                EXPAND_TEMPLATE_ARGUMENTS_A_R_ELSE_CLREF(BOOST_FUSION_N)
            >::type(EXPAND_PARAMETERS_A(BOOST_FUSION_N));
    }

#undef BOOST_FUSION_COMMA
#undef BOOST_FUSION_N
#endif
