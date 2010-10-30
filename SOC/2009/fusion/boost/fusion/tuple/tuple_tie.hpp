/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_TUPLE_TUPLE_TIE_HPP
#define BOOST_FUSION_TUPLE_TUPLE_TIE_HPP

#include <boost/fusion/support/internal/base.hpp>
#include <boost/fusion/tuple/tuple.hpp>

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   define BOOST_FUSION_SEQ_NAME(N) tuple
#else
#   define BOOST_FUSION_SEQ_NAME tuple
#endif
#define BOOST_FUSION_FUNCTION_NAME tie
#define BOOST_FUSION_MAX_SEQ_SIZE FUSION_MAX_VECTOR_SIZE
#include <boost/fusion/container/generation/detail/seq_tie.hpp>
#undef BOOST_FUSION_MAX_SEQ_SIZE
#undef BOOST_FUSION_FUNCTION_NAME
#undef BOOST_FUSION_SEQ_NAME

#endif
