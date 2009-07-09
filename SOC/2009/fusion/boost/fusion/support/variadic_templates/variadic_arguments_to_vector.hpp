// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_VARIADIC_TEMPLATES_VARIADIC_ARGUMENTS_TO_VECTOR_HPP
#define BOOST_FUSION_SUPPORT_VARIADIC_TEMPLATES_VARIADIC_ARGUMENTS_TO_VECTOR_HPP

//workaround to get variadic template arguments into fixed-length template
//argument list. We definitely need a boost config macro for this issue though.

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/mpl/vector.hpp>

namespace boost{namespace fusion
{
template<typename...> struct variadic_arguments_to_vector;
#define BOOST_PP_ITERATION_PARAMS_1\
    (3, (0, BOOST_MPL_LIMIT_VECTOR_SIZE, <boost/fusion/support/variadic_templates/detail/variadic_arguments_to_vector_n.hpp>))
#include BOOST_PP_ITERATE()

}}
#endif
