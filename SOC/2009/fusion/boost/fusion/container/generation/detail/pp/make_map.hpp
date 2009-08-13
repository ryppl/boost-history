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
#include <boost/preprocessor/arithmetic/mul.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        VARIADIC_TEMPLATE_WITH_DEFAULT(BOOST_PP_MUL(FUSION_MAX_MAP_SIZE,2))
        struct make_map;
    }
    
#define BOOST_PP_FILENAME_1 <boost/fusion/container/generation/detail/pp/make_map.hpp>
#define BOOST_PP_ITERATION_LIMITS (1, FUSION_MAX_MAP_SIZE)
#include BOOST_PP_ITERATE()

}}

#endif
#else

#define BOOST_FUSION_N BOOST_PP_ITERATION()

    namespace result_of
    {
        VARIADIC_TEMPLATE(BOOST_PP_MUL(BOOST_FUSION_N,2))
        struct make_map
        EXPAND_TEMPLATE_ARGUMENTS_SPECIALIZATION(
            BOOST_PP_MUL(FUSION_MAX_MAP_SIZE,2),
            BOOST_PP_MUL(BOOST_FUSION_N,2))
        {
#define BOOST_FUSION_MAP_ELEMENT(Z,INNER_N,_)\
            pair<\
                BOOST_PP_CAT(T,INNER_N),\
                typename detail::as_fusion_element<\
                    BOOST_PP_CAT(T,BOOST_PP_ADD(BOOST_FUSION_N,INNER_N))\
                >::type\
            >

            typedef map<BOOST_PP_ENUM(N, BOOST_FUSION_MAP_ELEMENT, _)> type;

#undef BOOST_FUSION_MAP_ELEMENT
        };
    }

#define BOOST_FUSION_MAKE_MAP_ARGS2(Z,INNER_N,_)\
    BOOST_PP_CAT(A,BOOST_PP_ADD(N,INNER_N))\
        BOOST_FUSION_R_ELSE_CLREF(BOOST_PP_EMPTY())

    VARIADIC_TEMPLATE_A(BOOST_PP_MUL(BOOST_FUSION_N,2))
    inline
#if N
        typename
#endif
        result_of::make_map<
            BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, A)
          , BOOST_PP_ENUM(BOOST_FUSION_N, BOOST_FUSION_MAKE_MAP_ARGS2, _)
        >::type

#define BOOST_FUSION_MAKE_MAP_ARGUMENTS(Z,INNER_N,__)\
    BOOST_PP_CAT(A,BOOST_PP_ADD(BOOST_FUSION_N,INNER_N))\
        BOOST_FUSION_R_ELSE_CLREF(BOOST_PP_EMPTY())\
    BOOST_PP_CAT(_,INNER_N)

    make_map(BOOST_PP_ENUM(BOOST_FUSION_N, BOOST_FUSION_MAKE_MAP_ARGUMENTS, _))

#undef BOOST_FUSION_MAKE_MAP_ARGUMENTS
    {
        return
#if N
            typename
#endif
            result_of::make_map<
                BOOST_PP_ENUM_PARAMS(BOOST_FUSION_N, A)
              , BOOST_PP_ENUM(BOOST_FUSION_N, BOOST_FUSION_MAKE_MAP_ARGS2, _)
            >::type(EXPAND_PARAMETERS_A(N));
    }

#undef BOOST_FUSION_MAKE_MAP_ARGS2

#undef BOOST_FUSION_N
#endif
