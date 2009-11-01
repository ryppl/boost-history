/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_GENERATION_MAKE_LIST_HPP
#define BOOST_FUSION_CONTAINER_GENERATION_MAKE_LIST_HPP

#include <boost/config.hpp>
#include <boost/fusion/container/list/list.hpp>

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   define BOOST_FUSION_SEQ_NAME(N) list
#else
#   define BOOST_FUSION_SEQ_NAME list
#endif
#define BOOST_FUSION_MAX_SEQ_SIZE FUSION_MAX_LIST_SIZE
#include <boost/fusion/container/generation/detail/make_seq.hpp>
#undef BOOST_FUSION_MAX_SEQ_SIZE
#undef BOOST_FUSION_SEQ_NAME

#endif
