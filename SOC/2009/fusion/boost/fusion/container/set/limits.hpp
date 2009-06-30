/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_SET_LIMITS_HPP
#define BOOST_FUSION_CONTAINER_SET_LIMITS_HPP

#include <boost/fusion/container/vector/limits.hpp>

#if defined(BOOST_NO_VARIADIC_TEMPLATES) || !defined(BOOST_FUSION_CPP0X_NO_DEPRECEATED)
#   if !defined(FUSION_MAX_SET_SIZE)
#       define FUSION_MAX_SET_SIZE FUSION_MAX_VECTOR_SIZE
#   else
#       if FUSION_MAX_SET_SIZE < 3
#           undef FUSION_MAX_SET_SIZE
#           if (FUSION_MAX_VECTOR_SIZE > 10)
#               define FUSION_MAX_SET_SIZE 10
#           else
#               define FUSION_MAX_SET_SIZE FUSION_MAX_VECTOR_SIZE
#           endif
#       endif
#   endif
#endif

#endif
