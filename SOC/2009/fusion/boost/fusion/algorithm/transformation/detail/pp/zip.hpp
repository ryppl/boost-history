/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_PP_ZIP_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_PP_ZIP_HPP

#include <boost/fusion/container/generation/vector_tie.hpp>
#include <boost/fusion/view/zip_view.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/template.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#ifdef BOOST_FUSION_ENABLE_STATIC_ASSERTS
#   include <boost/preprocessor/cat.hpp>
#   include <boost/preprocessor/repetition/repeat.hpp>
#endif

namespace boost { namespace fusion
{
    namespace result_of
    {
        VARIADIC_TEMPLATE_WITH_DEFAULT(FUSION_MAX_ZIP_SEQUENCES)
        struct zip;
    }

#define BOOST_PP_FILENAME_1 <boost/fusion/algorithm/transformation/detail/pp/zip_n.hpp>
#define BOOST_PP_ITERATION_LIMITS (2, FUSION_MAX_ZIP_SEQUENCES)
#include BOOST_PP_ITERATE()

}}

#endif
