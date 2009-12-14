/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_VECTOR_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_VECTOR_HPP

#include <boost/fusion/container/vector/vector_fwd.hpp>

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/container/vector/detail/pp/vector.hpp>
#else
#   include <boost/fusion/container/vector/vector_fwd.hpp>
#   include <boost/fusion/support/internal/ref.hpp>
#   ifdef BOOST_FUSION_TAGGED_VECTOR
#       include <boost/fusion/container/vector/detail/variadic_templates/tagged_impl.hpp>
#   endif

#   include <boost/fusion/container/vector/detail/at_impl.hpp>
#   include <boost/fusion/container/vector/detail/value_at_impl.hpp>
#   include <boost/fusion/container/vector/detail/begin_impl.hpp>
#   include <boost/fusion/container/vector/detail/end_impl.hpp>
#   include <boost/fusion/container/vector/detail/deref_impl.hpp>
#   include <boost/fusion/container/vector/detail/value_of_impl.hpp>

#   include <boost/fusion/container/vector/detail/variadic_templates/vector.hpp>
#endif

#endif
