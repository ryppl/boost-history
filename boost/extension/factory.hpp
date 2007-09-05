/*
 * Boost.Extension / factory:
 *         factory to register the implementations and create them
 *
 * (C) Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_EXTENSION_FACTORY_HPP
#define BOOST_EXTENSION_FACTORY_HPP

#include <boost/extension/common.hpp>

namespace boost{namespace extensions{
template <class T, class D
          BOOST_PP_COMMA_IF(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS)
          BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_PP_INC(\
          BOOST_EXTENSION_MAX_FUNCTOR_PARAMS), \
          class Param, void) >
  class create_function;
  
#define BOOST_EXTENSION_CREATE_FUNCTION_CLASS(Z, N, _) \
template <class T, class D BOOST_PP_COMMA_IF(N) \
          BOOST_PP_ENUM_PARAMS(N, \
          class Param) > \
class create_function<T, D BOOST_PP_COMMA_IF(N) \
                      BOOST_PP_ENUM_PARAMS(N, \
                     Param) > \
{ \
public: \
  static T * create(BOOST_PP_ENUM_BINARY_PARAMS(N, Param, p)) { \
    return new D(BOOST_PP_ENUM_PARAMS(N, p)); \
  } \
};

template <class T
          BOOST_PP_COMMA_IF(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS)
          BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_PP_INC(\
          BOOST_EXTENSION_MAX_FUNCTOR_PARAMS), \
          class Param, void) >
class factory;

#define BOOST_EXTENSION_FACTORY_CLASS(Z, N, _) \
template <class T BOOST_PP_COMMA_IF(N) \
          BOOST_PP_ENUM_PARAMS(N, \
          class Param) > \
class factory <T BOOST_PP_COMMA_IF(N) \
               BOOST_PP_ENUM_PARAMS(N, \
               Param) > \
{ \
public: \
  template <class D> \
  void set() { \
    func_ = &create_function<T, D BOOST_PP_COMMA_IF(N) \
    BOOST_PP_ENUM_PARAMS(N, \
    Param)>::create; \
  } \
  factory() : func_(0) {} \
  factory(const factory<T> & first) : func_(first.func_) {} \
  factory & operator=(const factory<T> & first) { \
    func_ = first->func_; \
    return *this; \
  } \
  bool is_valid() {return func_ != 0;} \
  T * create(BOOST_PP_ENUM_BINARY_PARAMS(N, Param, p)) { \
    if (func_) { \
      return func_(BOOST_PP_ENUM_PARAMS(N, p)); \
    } else { \
      return 0; \
    } \
  } \
private: \
  T * (*func_)(BOOST_PP_ENUM_PARAMS(N, Param)); \
};

BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS), \
                BOOST_EXTENSION_CREATE_FUNCTION_CLASS, _)
BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS), \
                BOOST_EXTENSION_FACTORY_CLASS, _)
#undef BOOST_EXTENSION_FACTORY_CLASS
#undef BOOST_EXTENSION_CREATE_FUNCTION_CLASS
}}

#endif // BOOST_EXTENSION_FACTORY_HPP
