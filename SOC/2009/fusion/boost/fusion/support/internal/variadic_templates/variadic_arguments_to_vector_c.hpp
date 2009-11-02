/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_VARIADIC_TEMPLATES_VARIADIC_ARGUMENTS_TO_VECTOR_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_VARIADIC_TEMPLATES_VARIADIC_ARGUMENTS_TO_VECTOR_HPP

//cschmdit: workaround to get variadic template arguments into a fixed-length
//template argument list. We definitely need a boost config macro for this issue
//though.

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

#include <boost/mpl/vector/vector10_c.hpp>
#include <boost/mpl/vector/vector20_c.hpp>
#include <boost/mpl/vector/vector30_c.hpp>
#include <boost/mpl/vector/vector40_c.hpp>
#include <boost/mpl/vector/vector50_c.hpp>

namespace boost { namespace fusion  { namespace detail
{
    template<int...>
    struct variadic_arguments_to_vector_c;

#define BOOST_PP_ITERATION_PARAMS_1\
    (3, (0, 50, <boost/fusion/support/internal/variadic_templates/detail/variadic_arguments_to_vector_c_n.hpp>))
#include BOOST_PP_ITERATE()
}}}

#endif
