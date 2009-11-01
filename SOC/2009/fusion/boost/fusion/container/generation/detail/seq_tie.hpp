/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/config.hpp>
#include <boost/fusion/container/generation/ignore.hpp>

#ifndef BOOST_FUSION_FUNCTION_NAME
#   include <boost/preprocessor/cat.hpp>

#   define BOOST_FUSION_UNDEF_FUNCTION_NAME
#   ifdef BOOST_NO_VARIADIC_TEMPLATES
#       include <boost/preprocessor/empty.hpp>

#       define BOOST_FUSION_FUNCTION_NAME BOOST_PP_CAT(BOOST_FUSION_SEQ_NAME(BOOST_PP_EMPTY()),_tie)
#   else
#       define BOOST_FUSION_FUNCTION_NAME BOOST_PP_CAT(BOOST_FUSION_SEQ_NAME,_tie)
#   endif
#endif

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/container/generation/detail/pp/seq_tie.hpp>
#else
#   include <boost/fusion/container/generation/detail/variadic_templates/seq_tie.hpp>
#endif

#ifdef BOOST_FUSION_UNDEF_FUNCTION_NAME
#   undef BOOST_FUSION_FUNCTION_NAME
#   undef BOOST_FUSION_UNDEF_FUNCTION_NAME
#endif
