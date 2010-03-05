/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_DETAIL_PP_VECTOR_N_CHOOSER_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_DETAIL_PP_VECTOR_N_CHOOSER_HPP

#include <boost/fusion/container/vector/limits.hpp>
//  include vector0..N where N is FUSION_MAX_VECTOR_SIZE
#include <boost/fusion/container/vector/vector10.hpp>
#if (FUSION_MAX_VECTOR_SIZE > 10)
#    include <boost/fusion/container/vector/vector20.hpp>
#endif
#if (FUSION_MAX_VECTOR_SIZE > 20)
#   include <boost/fusion/container/vector/vector30.hpp>
#endif
#if (FUSION_MAX_VECTOR_SIZE > 30)
#    include <boost/fusion/container/vector/vector40.hpp>
#endif
#if (FUSION_MAX_VECTOR_SIZE > 40)
#    include <boost/fusion/container/vector/vector50.hpp>
#endif
#include <boost/fusion/support/internal/template.hpp>

#include <boost/preprocessor/cat.hpp>

namespace boost { namespace fusion { namespace detail
{
    VARIADIC_TEMPLATE_WITH_DEFAULT(FUSION_MAX_VECTOR_SIZE)
    struct vector_n_chooser;

#define BOOST_PP_FILENAME_1 <boost/fusion/container/vector/detail/pp/vector_n_chooser_n.hpp>
#define BOOST_PP_ITERATION_LIMITS (0, FUSION_MAX_VECTOR_SIZE)
#include BOOST_PP_ITERATE()

}}}

#endif
