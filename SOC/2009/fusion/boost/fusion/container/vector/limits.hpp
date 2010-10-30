/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_LIMITS_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_LIMITS_HPP

#include <boost/fusion/support/internal/base.hpp>

#if defined(BOOST_NO_VARIADIC_TEMPLATES) || !defined(BOOST_FUSION_CPP0X_NO_DEPRECEATED)
#   ifndef FUSION_MAX_VECTOR_SIZE
#       define FUSION_MAX_VECTOR_SIZE 10
#   else
#       if FUSION_MAX_VECTOR_SIZE < 3
#           undef FUSION_MAX_VECTOR_SIZE
#           define FUSION_MAX_VECTOR_SIZE 10
#       endif
#   endif
#endif

#endif
