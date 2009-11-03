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

#include <boost/mpl/vector/vector10.hpp>
#include <boost/mpl/vector/vector20.hpp>
#include <boost/mpl/vector/vector30.hpp>
#include <boost/mpl/vector/vector40.hpp>
#include <boost/mpl/vector/vector50.hpp>

namespace boost { namespace fusion  { namespace detail
{
    template<typename...>
    struct variadic_arguments_to_vector;

#define BOOST_PP_FILENAME_1\
    <boost/fusion/support/internal/variadic_templates/detail/variadic_arguments_to_vector_n.hpp>
#define BOOST_PP_ITERATION_LIMITS (0, 50)
#include BOOST_PP_ITERATE()
}}}

#endif
