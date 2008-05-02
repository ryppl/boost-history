/*
 * Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_REFLECTION_FUNCTION_HPP
#define BOOST_REFLECTION_FUNCTION_HPP

#include <boost/reflection/instance.hpp>

namespace boost {
namespace reflections {
namespace impl {
typedef void (instance::*MemberFunctionPtr)();
}  // namespace impl
template <class ReturnValue = void
          BOOST_PP_COMMA_IF(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS)
          BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_PP_INC
          (BOOST_REFLECTION_MAX_FUNCTOR_PARAMS), class Param, void)>
class function;

#define BOOST_PP_ITERATION_LIMITS (0, \
    BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS) - 1)
#define BOOST_PP_FILENAME_1 <boost/reflection/impl/function.hpp>
#include BOOST_PP_ITERATE()
}  // namespace reflections
}  // namespace boost
#endif
