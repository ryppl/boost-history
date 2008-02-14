/*
 * Boost.Reflection / main header
 *
 * (C) Copyright Mariano G. Consoni and Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_REFLECTION_REFLECTION_HPP
#define BOOST_REFLECTION_REFLECTION_HPP
#include <map>
#include <vector>

#include <boost/extension/impl/typeinfo.hpp>
#include <boost/reflection/constructor.hpp>
#include <boost/reflection/factory.hpp>
#include <boost/reflection/function.hpp>
// #include <boost/reflection/parameter_map.hpp>

namespace boost {namespace reflections {
using extensions::type_info_handler;

#define BOOST_REFLECTION_REFLECTION_GET_CONSTRUCTOR_FUNCTION(Z, N, _) \
template <class ParamFirst BOOST_PP_COMMA_IF(N) \
  BOOST_PP_ENUM_PARAMS(N, class Param)> \
instance_constructor<ParamFirst  BOOST_PP_COMMA_IF(N) \
            BOOST_PP_ENUM_PARAMS(N, Param)> get_constructor() { \
  constructor_info t(reflections::type_info_handler<TypeInfo, \
  instance (*)(ParamFirst BOOST_PP_COMMA_IF(N) \
               BOOST_PP_ENUM_PARAMS(N, Param))>::get_class_type()); \
  typename std::map<constructor_info, FunctionPtr>::iterator it = \
    constructors_.find(t); \
  if (it == constructors_.end()) { \
    return instance_constructor<ParamFirst  BOOST_PP_COMMA_IF(N) \
                       BOOST_PP_ENUM_PARAMS(N, Param)>(); \
  } else { \
    return reinterpret_cast<instance (*)(ParamFirst BOOST_PP_COMMA_IF(N) \
                                         BOOST_PP_ENUM_PARAMS(N, Param))> \
    (it->second); \
  } \
}

#define BOOST_REFLECTION_REFLECTION_GET_FUNCTION_FUNCTION(Z, N, _) \
template <class ReturnValue BOOST_PP_COMMA_IF(N) \
          BOOST_PP_ENUM_PARAMS(N, class Param)> \
function<ReturnValue BOOST_PP_COMMA_IF(N) \
         BOOST_PP_ENUM_PARAMS(N, Param)> get_function(Info info) { \
  function_info f(reflections::type_info_handler<TypeInfo, \
                  ReturnValue (*)(BOOST_PP_ENUM_PARAMS(N, Param))> \
                  ::get_class_type(), info); \
  typename std::map<function_info, \
    std::pair<MemberFunctionPtr, \
    FunctionPtr> >::iterator it = \
    functions_.find(f); \
  if (it == functions_.end()) { \
    return function<ReturnValue BOOST_PP_COMMA_IF(N) \
                    BOOST_PP_ENUM_PARAMS(N, Param)>(); \
  } else { \
    return function<ReturnValue BOOST_PP_COMMA_IF(N) \
    BOOST_PP_ENUM_PARAMS(N, Param)> \
    (reinterpret_cast<ReturnValue (*)(void *, MemberFunctionPtr \
                                      BOOST_PP_COMMA_IF(N) \
                                      BOOST_PP_ENUM_PARAMS(N, Param))> \
     (it->second.second), it->second.first); \
  } \
}
  
  

typedef void (*FunctionPtr)();
/*
template <class Info = std::string,
          class TypeInfo = reflections::default_type_info>
class parameter_info {
public:
  virtual size_t num_parameters() = 0;
  virtual const Info & operator[](unsigned position) = 0;
  virtual TypeInfo type() = 0;
  template <class T = void, class S = void>
    class specific_parameter_info : public parameter_info {
    public:
      specific_parameter_info() {
      }
    private:
    };
  
  template <class T>
  class specific_parameter_info<T> : public parameter_info {
  public:
    specific_parameter_info() {
    }
  private:
  };
  specific_parameter_info<int> * create() {
    return new specific_parameter_info<int>();
  }
private:
  
};*/

template<class Info, class TypeInfo, class ParameterInfo = void>
struct basic_function_info {
  TypeInfo type_info_;
  Info info_;
  std::vector<ParameterInfo> parameter_info_;
  basic_function_info(TypeInfo t, Info i) : type_info_(t), info_(i)
  {
  }
  basic_function_info(const basic_function_info & s) 
    : type_info_(s.type_info_), info_(s.info_) {}
  basic_function_info & operator=(basic_function_info & s) {
    type_info_ = s.type_info_;
    info_ = s.info_;
  }
  friend inline bool operator<(const basic_function_info & t,
                               const basic_function_info & s) {
    return t.type_info_ < s.type_info_ ||
    (t.type_info_ == s.type_info_ &&
     t.info_ < s.info_);
  }
};

template<class Info, class TypeInfo>
struct basic_function_info<Info, TypeInfo> {
  TypeInfo type_info_;
  Info info_;
  basic_function_info(TypeInfo t, Info i) : type_info_(t), info_(i)
  {
  }
  basic_function_info(const basic_function_info & s) 
    : type_info_(s.type_info_), info_(s.info_) {}
  basic_function_info & operator=(basic_function_info & s) {
    type_info_ = s.type_info_;
    info_ = s.info_;
  }
  friend inline bool operator<(const basic_function_info & t,
                               const basic_function_info & s) {
    return t.type_info_ < s.type_info_ ||
    (t.type_info_ == s.type_info_ &&
     t.info_ < s.info_);
  }
};

template<class TypeInfo, class ParameterInfo = void>
struct basic_constructor_info {
  TypeInfo type_info_;
  std::vector<ParameterInfo> parameter_info_;
  basic_constructor_info(TypeInfo t) : type_info_(t)
  {
  }
  basic_constructor_info(const basic_constructor_info & s) 
    : type_info_(s.type_info_) {}
  basic_constructor_info & operator=(basic_constructor_info & s) {
    type_info_ = s.type_info_;
  }
  friend inline bool operator<(const basic_constructor_info & t,
                               const basic_constructor_info & s) {
    return t.type_info_ < s.type_info_;
  }
};

template<class TypeInfo>
struct basic_constructor_info<TypeInfo> {
  TypeInfo type_info_;
  basic_constructor_info(TypeInfo t) : type_info_(t)
  {
  }
  basic_constructor_info(const basic_constructor_info & s) 
    : type_info_(s.type_info_) {}
  basic_constructor_info & operator=(basic_constructor_info & s) {
    type_info_ = s.type_info_;
  }
  friend inline bool operator<(const basic_constructor_info & t,
                               const basic_constructor_info & s) {
    return t.type_info_ < s.type_info_;
  }
};

template <class Info = std::string, class ParameterInfo = void,
          class TypeInfo = extensions::default_type_info>
class basic_reflection
{
public:
  template <class Q, class R, class S, class T>
  friend class reflector;
  BOOST_PP_REPEAT(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS, \
                  BOOST_REFLECTION_REFLECTION_GET_CONSTRUCTOR_FUNCTION, _)
  instance_constructor<> get_constructor() {
    constructor_info t(reflections::type_info_handler<TypeInfo,
    instance (*)()>::get_class_type());
    typename std::map<constructor_info, FunctionPtr>::iterator it = 
      constructors_.find(t);
    if (it == constructors_.end()) {
      return instance_constructor<>();
    } else {
      return reinterpret_cast<instance (*)()>(it->second);
    }
  }
  BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS), \
                  BOOST_REFLECTION_REFLECTION_GET_FUNCTION_FUNCTION, _)
private:
  typedef basic_function_info<Info, TypeInfo, ParameterInfo> function_info;
  typedef basic_constructor_info<TypeInfo, ParameterInfo> constructor_info;
  std::map<constructor_info, FunctionPtr> constructors_;
  std::map<function_info, 
    std::pair<MemberFunctionPtr, FunctionPtr> > functions_;
};
template <class Info, class TypeInfo>
class basic_reflection<Info, void, TypeInfo>
{
public:
  template <class Q, class R, class S, class T>
  friend class reflector;
  BOOST_PP_REPEAT(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS, \
                  BOOST_REFLECTION_REFLECTION_GET_CONSTRUCTOR_FUNCTION, _)
    instance_constructor<> get_constructor() {
      constructor_info t(reflections::type_info_handler<TypeInfo,
      instance (*)()>::get_class_type());
      typename std::map<constructor_info, FunctionPtr>::iterator it =
        constructors_.find(t);
      if (it == constructors_.end()) {
        return instance_constructor<>();
      } else {
        return reinterpret_cast<instance (*)()>(it->second);
      }
    }
  BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS), \
                  BOOST_REFLECTION_REFLECTION_GET_FUNCTION_FUNCTION, _)
private:
  typedef basic_function_info<Info, TypeInfo> function_info;
  typedef basic_constructor_info<TypeInfo> constructor_info;
  std::map<constructor_info, FunctionPtr> constructors_;
  std::map<function_info, 
    std::pair<MemberFunctionPtr, FunctionPtr> > functions_;
};

typedef basic_reflection<> reflection;
}}
#endif // BOOST_REFLECTION_REFLECTION_HPP
