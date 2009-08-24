/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_PP_IS_ITERATING
#   ifndef BOOST_FUSION_CONTAINER_VECTOR_DETAIL_PP_VECTOR_N_CHOOSER_HPP
#   define BOOST_FUSION_CONTAINER_VECTOR_DETAIL_PP_VECTOR_N_CHOOSER_HPP

#   include <boost/fusion/container/vector/limits.hpp>
//  include vector0..N where N is FUSION_MAX_VECTOR_SIZE
#   include <boost/fusion/container/vector/vector10.hpp>
#   if (FUSION_MAX_VECTOR_SIZE > 10)
#       include <boost/fusion/container/vector/vector20.hpp>
#   endif
#   if (FUSION_MAX_VECTOR_SIZE > 20)
#       include <boost/fusion/container/vector/vector30.hpp>
#   endif
#   if (FUSION_MAX_VECTOR_SIZE > 30)
#       include <boost/fusion/container/vector/vector40.hpp>
#   endif
#   if (FUSION_MAX_VECTOR_SIZE > 40)
#       include <boost/fusion/container/vector/vector50.hpp>
#   endif
#   include <boost/fusion/support/internal/template.hpp>

#   include <boost/preprocessor/cat.hpp>

namespace boost { namespace fusion { namespace detail
{
    VARIADIC_TEMPLATE_WITH_DEFAULT(FUSION_MAX_VECTOR_SIZE)
    struct vector_n_chooser;

#   define BOOST_PP_FILENAME_1 <boost/fusion/container/vector/detail/pp/vector_n_chooser.hpp>
#   define BOOST_PP_ITERATION_LIMITS (0, FUSION_MAX_VECTOR_SIZE)
#   include BOOST_PP_ITERATE()

}}}

#   endif
#else
#   define BOOST_FUSION_N BOOST_PP_ITERATION()

    VARIADIC_TEMPLATE(BOOST_FUSION_N)
    struct vector_n_chooser
        EXPAND_TEMPLATE_ARGUMENTS_SPECIALIZATION(
            FUSION_MAX_VECTOR_SIZE,BOOST_FUSION_N)
    {
        typedef
            BOOST_PP_CAT(vector, BOOST_FUSION_N)<
                EXPAND_TEMPLATE_ARGUMENTS(BOOST_FUSION_N)
            >
        type;
    };

#   undef BOOST_FUSION_N
#endif
