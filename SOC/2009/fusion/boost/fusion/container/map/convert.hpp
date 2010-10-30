/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_MAP_CONVERT_HPP
#define BOOST_FUSION_CONTAINER_MAP_CONVERT_HPP

#include <boost/fusion/support/internal/base.hpp>
#include <boost/fusion/container/map/map.hpp>
#include <boost/fusion/container/generation/make_map.hpp>

#define BOOST_FUSION_SEQ_NAME map
#define BOOST_FUSION_MAX_SEQ_SIZE FUSION_MAX_MAP_SIZE
#include <boost/fusion/container/detail/convert_impl.hpp>
#undef BOOST_FUSION_MAX_SEQ_SIZE
#undef BOOST_FUSION_SEQ_NAME

#endif
