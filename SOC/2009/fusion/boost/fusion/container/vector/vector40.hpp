/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_VECTOR40_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_VECTOR40_HPP

#include <boost/fusion/support/internal/base.hpp>
#ifdef BOOST_FUSION_PREFER_MPL
#   include <boost/mpl/vector/vector40.hpp>
#endif

#if defined(BOOST_NO_VARIADIC_TEMPLATES) || !defined(BOOST_FUSION_CPP0X_NO_DEPRECEATED)
#   define BOOST_FUSION_FROM 31
#   define BOOST_FUSION_TO 40

#   ifdef BOOST_NO_VARIADIC_TEMPLATES
#       include<boost/fusion/container/vector/detail/pp/vector_n.hpp>
#   else
#       include<boost/fusion/container/vector/detail/variadic_templates/vector_n.hpp>
#   endif

#   undef BOOST_FUSION_FROM
#   undef BOOST_FUSION_TO
#else
#   include <boost/fusion/container/vector/vector.hpp>
#endif

#endif
