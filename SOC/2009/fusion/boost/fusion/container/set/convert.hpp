/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_SET_CONVERT_HPP
#define BOOST_FUSION_CONTAINER_SET_CONVERT_HPP

#include <boost/fusion/container/set/set.hpp>

#define BOOST_FUSION_SEQ_NAME set
#define BOOST_FUSION_MAX_SEQ_SIZE FUSION_MAX_SET_SIZE
#include <boost/fusion/container/detail/convert_impl.hpp>
#undef BOOST_FUSION_MAX_SEQ_SIZE
#undef BOOST_FUSION_SEQ_NAME

#endif
