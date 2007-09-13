/*
 * Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_EXTENSION_CONSTRUCTOR_HPP
#define BOOST_EXTENSION_CONSTRUCTOR_HPP
#include <boost/reflection/instance.hpp>
namespace boost {namespace reflections {
template <BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_PP_INC(\
          BOOST_EXTENSION_MAX_FUNCTOR_PARAMS), class Param, void)>
class constructor;

#define BOOST_REFLECTION_CONSTRUCTOR_CLASS(Z, N, _) \
template <BOOST_PP_ENUM_PARAMS(N, class Param)> \
class constructor<BOOST_PP_ENUM_PARAMS(N, Param)> { \
public: \
  constructor(instance (*func)(BOOST_PP_ENUM_PARAMS(N, Param)) = 0) \
  : func_(func) { \
  } \
  instance call(BOOST_PP_ENUM_BINARY_PARAMS(N, Param, p)) { \
    return (*func_)(BOOST_PP_ENUM_PARAMS(N, p)); \
  } \
  instance operator()(BOOST_PP_ENUM_BINARY_PARAMS(N, Param, p)) { \
    return (*func_)(BOOST_PP_ENUM_PARAMS(N, p)); \
  } \
  bool valid() {return func_ != 0;} \
  private: \
  instance (*func_)(BOOST_PP_ENUM_PARAMS(N, Param)); \
};

BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS), \
                BOOST_REFLECTION_CONSTRUCTOR_CLASS, _)
}}
#endif
