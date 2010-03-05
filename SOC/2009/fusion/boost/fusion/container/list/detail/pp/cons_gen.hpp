/*=============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_PP_CONS_GEN_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_PP_CONS_GEN_HPP

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>

namespace boost { namespace fusion { namespace detail
{
    VARIADIC_TEMPLATE_WITH_DEFAULT(FUSION_MAX_LIST_SIZE)
    struct cons_gen;

#define BOOST_PP_FILENAME_1 <boost/fusion/container/list/detail/pp/cons_gen_n.hpp>
#define BOOST_PP_ITERATION_LIMITS (0, FUSION_MAX_LIST_SIZE)
#include BOOST_PP_ITERATE()
}}}

#endif

