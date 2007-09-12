/*
 * Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_EXTENSION_FUNCTION_HPP
#define BOOST_EXTENSION_FUNCTION_HPP

#include <boost/reflection/instance.hpp>

namespace boost {namespace reflections {

typedef void (instance::*MemberFunctionPtr)();
template <class ReturnValue = void 
          BOOST_PP_COMMA_IF(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS)
          BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_PP_INC
          (BOOST_EXTENSION_MAX_FUNCTOR_PARAMS), class Param, void)>
class function;
  
#define BOOST_REFLECTION_FUNCTION_CLASS(Z, N, _) \
template <class ReturnValue  BOOST_PP_COMMA_IF(N) \
  BOOST_PP_ENUM_PARAMS(N, class Param)> \
class function<ReturnValue BOOST_PP_COMMA_IF(N) \
               BOOST_PP_ENUM_PARAMS(N, Param)> { \
public: \
  function(ReturnValue (*func)(void *, MemberFunctionPtr \
                        BOOST_PP_COMMA_IF(N) \
                        BOOST_PP_ENUM_PARAMS(N, Param)) = 0, \
           MemberFunctionPtr member_function = 0) \
  : func_(func), \
    member_function_(member_function) { \
  } \
  ReturnValue call(instance & inst BOOST_PP_COMMA_IF(N) \
                   BOOST_PP_ENUM_BINARY_PARAMS(N, Param, p)) { \
    return (*func_)(inst.val_, member_function_ BOOST_PP_COMMA_IF(N) \
                    BOOST_PP_ENUM_PARAMS(N, p)); \
  } \
  ReturnValue operator()(instance & inst BOOST_PP_COMMA_IF(N) \
                         BOOST_PP_ENUM_BINARY_PARAMS(N, Param, p)) { \
    return (*func_)(inst.val_, member_function_ BOOST_PP_COMMA_IF(N) \
                    BOOST_PP_ENUM_PARAMS(N, p)); \
  } \
  bool valid() { \
    return member_function_ != 0 && func_ != 0; \
  } \
private: \
  ReturnValue (*func_)(void *, MemberFunctionPtr \
                       BOOST_PP_COMMA_IF(N) \
                       BOOST_PP_ENUM_PARAMS(N, Param)); \
  MemberFunctionPtr member_function_; \
};


BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS), \
                BOOST_REFLECTION_FUNCTION_CLASS, _)
}}
#endif