/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_GENERATION_DETAIL_PP_MAP_TIE_HPP
#define BOOST_FUSION_CONTAINER_GENERATION_DETAIL_PP_MAP_TIE_HPP

#include <boost/fusion/container/map/map.hpp>
#include <boost/fusion/support/deduce.hpp>
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
        struct map_tie;
    }
    
#define BOOST_PP_FILENAME_1 <boost/fusion/container/generation/detail/pp/map_tie_n.hpp>
#define BOOST_PP_ITERATION_LIMITS (0, FUSION_MAX_MAP_SIZE)
#include BOOST_PP_ITERATE()

}}

#endif
