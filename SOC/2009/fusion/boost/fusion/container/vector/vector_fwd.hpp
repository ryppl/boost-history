/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_VECTOR_FWD_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_VECTOR_FWD_HPP

#include <boost/config.hpp>
#include <boost/fusion/container/vector/limits.hpp>
#include <boost/fusion/support/internal/template.hpp>

namespace boost { namespace fusion
{
    struct vector_tag;
    struct vector_iterator_tag;
    struct random_access_traversal_tag;

#if defined(BOOST_FUSION_TAGGED_VECTOR)
#   if defined(BOOST_NO_VARIADIC_TEMPLATES) || defined(BOOST_NO_DECLTYPE)
#       error Your compiler does not support a tagged implementation
#   endif
#elif !defined(BOOST_FUSION_PREFER_MPL)
    template<typename Vector, int I>
    struct vector_meta_value_at;
#endif

    VARIADIC_TEMPLATE_WITH_DEFAULT(FUSION_MAX_VECTOR_SIZE)
    struct vector;
}}

#endif
