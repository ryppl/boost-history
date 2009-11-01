/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_GENERATION_VECTOR_TIE_HPP
#define BOOST_FUSION_CONTAINER_GENERATION_VECTOR_TIE_HPP

#include <boost/config.hpp>
#include <boost/fusion/container/vector/vector.hpp>

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/preprocessor/cat.hpp>

#   define BOOST_FUSION_SEQ_NAME(N) BOOST_PP_CAT(vector,N)
#else
#   define BOOST_FUSION_SEQ_NAME vector
#endif
#define BOOST_FUSION_MAX_SEQ_SIZE FUSION_MAX_VECTOR_SIZE
#include <boost/fusion/container/generation/detail/seq_tie.hpp>
#undef BOOST_FUSION_MAX_SEQ_SIZE
#undef BOOST_FUSION_SEQ_NAME

#endif
