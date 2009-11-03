/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_CONVERT_HPP
#define BOOST_FUSION_CONTAINER_LIST_CONVERT_HPP

#include <boost/fusion/container/list/list.hpp>
#include <boost/fusion/container/list/cons.hpp>

#define BOOST_FUSION_SEQ_NAME list
#define BOOST_FUSION_MAX_SEQ_SIZE FUSION_MAX_LIST_SIZE
#include <boost/fusion/container/detail/convert_impl.hpp>
#undef BOOST_FUSION_MAX_SEQ_SIZE
#undef BOOST_FUSION_SEQ_NAME

#include <boost/fusion/container/list/detail/convert_impl.hpp>

#endif
