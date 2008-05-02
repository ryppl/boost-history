/*
 * Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_REFLECTION_FACTORY_HPP
#define BOOST_REFLECTION_FACTORY_HPP
#include <boost/reflection/common.hpp>
namespace boost {namespace reflections {
#define BOOST_REFLECTION_CONSTRUCT_FUNCTION(Z, N, _) \
template <class Actual \
BOOST_PP_COMMA_IF(N) \
BOOST_PP_ENUM_PARAMS(N, class Param) > \
Actual * construct(BOOST_PP_ENUM_BINARY_PARAMS(N, Param, p)) { \
  return new Actual(BOOST_PP_ENUM_PARAMS(N, p)); \
}

#define BOOST_REFLECTION_CONSTRUCTI_FUNCTION(Z, N, _) \
template <class Interface, class Actual \
BOOST_PP_COMMA_IF(N) \
BOOST_PP_ENUM_PARAMS(N, class Param) > \
Interface * construct_interface(BOOST_PP_ENUM_BINARY_PARAMS(N, Param, p)) { \
  return new Actual(BOOST_PP_ENUM_PARAMS(N, p)); \
}
BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS), \
                BOOST_REFLECTION_CONSTRUCT_FUNCTION, _)
BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS), \
                BOOST_REFLECTION_CONSTRUCTI_FUNCTION, _)

}}
#endif
