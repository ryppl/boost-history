/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_VECTOR20_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_VECTOR20_HPP

#include <boost/config.hpp>

#if defined(BOOST_NO_VARIADIC_TEMPLATES) || !defined(BOOST_FUSION_CPP0X_NO_DEPRECEATED)
#ifdef BOOST_NO_VARIADIC_TEMPLATES

#else
#   include <boost/fusion/container/vector/vector.hpp>
#endif

namespace boost { namespace fusion
{
    struct vector_tag;
    struct fusion_sequence_tag;
    struct random_access_traversal_tag;

// expand vector11 to vector20
#   ifdef BOOST_NO_VARIADIC_TEMPLATES
#       define BOOST_PP_FILENAME_1 <boost/fusion/container/vector/detail/pp/vector_n.hpp>
#   else
#       define BOOST_PP_FILENAME_1 <boost/fusion/container/vector/detail/variadic_templates/vector_n.hpp>
#   endif
#   define BOOST_PP_ITERATION_LIMITS (11, 20)
#   include BOOST_PP_ITERATE()

}}
#else
#   include <boost/fusion/container/vector/vector.hpp>
#endif
#endif
