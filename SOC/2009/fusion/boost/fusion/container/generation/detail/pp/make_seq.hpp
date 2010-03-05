/*=============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/fusion/support/deduce.hpp>
#include <boost/fusion/support/internal/template.hpp>

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/empty.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        VARIADIC_TEMPLATE_WITH_DEFAULT(BOOST_FUSION_MAX_SEQ_SIZE)
        struct BOOST_PP_CAT(make_,BOOST_FUSION_SEQ_NAME(BOOST_PP_EMPTY()));
    }

#define BOOST_PP_FILENAME_1 <boost/fusion/container/generation/detail/pp/make_seq_n.hpp>
#define BOOST_PP_ITERATION_LIMITS (0, BOOST_FUSION_MAX_SEQ_SIZE)
#include BOOST_PP_ITERATE()
}}
