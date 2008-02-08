/*
 * Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_REFLECTION_REFLECTOR_HPP
#define BOOST_REFLECTION_REFLECTOR_HPP

#include <boost/reflection/common.hpp>
#include <boost/reflection/factory.hpp>
#include <boost/reflection/function.hpp>
#include <boost/reflection/constructor.hpp>
#include <boost/reflection/instance.hpp>
#include <boost/reflection/reflection.hpp>

namespace boost {namespace reflections {


#define BOOST_REFLECTION_REFLECTOR_REFLECT_CONSTRUCTOR_FUNCTION(Z, N, _) \
  template <class ParamFirst BOOST_PP_COMMA_IF(N) \
  BOOST_PP_ENUM_PARAMS(N, class Param)> \
void reflect_constructor() { \
  add_constructor<ParamFirst BOOST_PP_COMMA_IF(N) \
  BOOST_PP_ENUM_PARAMS(N, Param)>(&construct_instance<T, ParamFirst \
                                  BOOST_PP_COMMA_IF(N) \
                                  BOOST_PP_ENUM_PARAMS(N, Param)>); \
}
#define BOOST_REFLECTION_REFLECTOR_REFLECT_FUNCTION(Z, N, _) \
template <class ReturnValue BOOST_PP_COMMA_IF(N) \
          BOOST_PP_ENUM_PARAMS(N, class Param)> \
void reflect(ReturnValue (T::*func)(BOOST_PP_ENUM_PARAMS(N, Param)), \
             Info info) { \
  function_info f(reflections::type_info_handler<TypeInfo, \
                  ReturnValue (*)(BOOST_PP_ENUM_PARAMS(N, Param))> \
                    ::get_class_type(), info); \
  ReturnValue (*f2)(void *, MemberFunctionPtr BOOST_PP_COMMA_IF(N) \
      BOOST_PP_ENUM_PARAMS(N, Param)) = \
      &call_member<T, ReturnValue BOOST_PP_COMMA_IF(N) \
                   BOOST_PP_ENUM_PARAMS(N, Param)>; \
  std::pair<MemberFunctionPtr, FunctionPtr> \
    p(reinterpret_cast<MemberFunctionPtr>(func), \
      reinterpret_cast<FunctionPtr>(f2)); \
  std::pair<function_info, std::pair<MemberFunctionPtr, FunctionPtr> > \
    p2(f, p); \
  reflection_->functions_.insert(p2); \
}
#define BOOST_REFLECTION_REFLECTOR_ADD_CONSTRUCTOR_FUNCTION(Z, N, _) \
template <class ParamFirst BOOST_PP_COMMA_IF(N) \
BOOST_PP_ENUM_PARAMS(N, class Param)> \
void add_constructor(instance (*func)(ParamFirst BOOST_PP_COMMA_IF(N) \
                                      BOOST_PP_ENUM_PARAMS(N, Param))) { \
  reflection_->constructors_.insert(std::make_pair<TypeInfo, FunctionPtr>( \
      reflections::type_info_handler \
      <TypeInfo, instance (*)(ParamFirst BOOST_PP_COMMA_IF(N) \
                              BOOST_PP_ENUM_PARAMS(N, Param))> \
        ::get_class_type(), \
      reinterpret_cast<FunctionPtr>(func))); \
}

// Versions with included info about parameters
#define BOOST_REFLECTION_PUSH_PARAMETER_INFO_SINGLE(f, N) \
(f).parameter_info_.push_back(BOOST_PP_CAT(i, N));

#define BOOST_REFLECTION_PUSH_PARAMETER_INFO(f, N) \
BOOST_PP_IF(N, BOOST_REFLECTION_PUSH_PARAMETER_INFO_SINGLE(f, BOOST_PP_DEC(N)),) \
BOOST_PP_IF(BOOST_PP_DEC(N), BOOST_REFLECTION_PUSH_PARAMETER_INFO(f, BOOST_PP_DEC(N)),)

#define BOOST_REFLECTION_REFLECTOR_REFLECT_CONSTRUCTOR_INFO_FUNCTION(Z, N, _) \
  template <class ParamFirst BOOST_PP_COMMA_IF(N) \
  BOOST_PP_ENUM_PARAMS(N, class Param)> \
void reflect_constructor(BOOST_PP_ENUM_PARAMS(N, ParameterInfo i)) { \
  add_constructor<ParamFirst BOOST_PP_COMMA_IF(N) \
  BOOST_PP_ENUM_PARAMS(N, Param)>(&construct_instance<T, ParamFirst \
                                  BOOST_PP_COMMA_IF(N) \
                                  BOOST_PP_ENUM_PARAMS(N, Param)> \
                                  BOOST_PP_COMMA_IF(N) \
                                  BOOST_PP_ENUM_PARAMS(N, i)); \
}
#define BOOST_REFLECTION_REFLECTOR_REFLECT_INFO_FUNCTION(Z, N, _) \
template <class ReturnValue BOOST_PP_COMMA_IF(N) \
          BOOST_PP_ENUM_PARAMS(N, class Param)> \
void reflect(ReturnValue (T::*func)(BOOST_PP_ENUM_PARAMS(N, Param)), \
             Info info, ParameterInfo i_return BOOST_PP_COMMA_IF(N) \
             BOOST_PP_ENUM_PARAMS(N, ParameterInfo i)) { \
  function_info f(reflections::type_info_handler<TypeInfo, \
                  ReturnValue (*)(BOOST_PP_ENUM_PARAMS(N, Param))> \
                    ::get_class_type(), info); \
  BOOST_REFLECTION_PUSH_PARAMETER_INFO(f, N);  \
  f.parameter_info_.push_back(i_return); \
  ReturnValue (*f2)(void *, MemberFunctionPtr BOOST_PP_COMMA_IF(N) \
      BOOST_PP_ENUM_PARAMS(N, Param)) = \
      &call_member<T, ReturnValue BOOST_PP_COMMA_IF(N) \
                   BOOST_PP_ENUM_PARAMS(N, Param)>; \
  std::pair<MemberFunctionPtr, FunctionPtr> \
    in_pair(reinterpret_cast<MemberFunctionPtr>(func), \
      reinterpret_cast<FunctionPtr>(f2)); \
  std::pair<function_info, std::pair<MemberFunctionPtr, FunctionPtr> > \
    out_pair(f, in_pair); \
  reflection_->functions_.insert(out_pair); \
}
#define BOOST_REFLECTION_REFLECTOR_REFLECT_INFO_VOID_RET_FUNCTION(Z, N, _) \
template <class ParamFirst BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, class Param)> \
void reflect(void (T::*func)(ParamFirst p_first BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, Param)), \
             Info info, ParameterInfo i_first BOOST_PP_COMMA_IF(N) \
             BOOST_PP_ENUM_PARAMS(N, ParameterInfo i)) { \
  function_info f(reflections::type_info_handler<TypeInfo, \
                  void (*)(BOOST_PP_ENUM_PARAMS(N, Param))> \
                    ::get_class_type(), info); \
  f.parameter_info_.push_back(i_first); \
  BOOST_REFLECTION_PUSH_PARAMETER_INFO(f, N);  \
  void (*f2)(void *, MemberFunctionPtr BOOST_PP_COMMA_IF(N) \
      BOOST_PP_ENUM_PARAMS(N, Param)) = \
      &call_member<T, void BOOST_PP_COMMA_IF(N) \
                   BOOST_PP_ENUM_PARAMS(N, Param)>; \
  std::pair<MemberFunctionPtr, FunctionPtr> \
    in_pair(reinterpret_cast<MemberFunctionPtr>(func), \
      reinterpret_cast<FunctionPtr>(f2)); \
  std::pair<function_info, std::pair<MemberFunctionPtr, FunctionPtr> > \
    out_pair(f, in_pair); \
  reflection_->functions_.insert(out_pair); \
}
#define BOOST_REFLECTION_REFLECTOR_ADD_CONSTRUCTOR_INFO_FUNCTION(Z, N, _) \
template <class ParamFirst BOOST_PP_COMMA_IF(N) \
BOOST_PP_ENUM_PARAMS(N, class Param)> \
void add_constructor(instance (*func)(ParamFirst BOOST_PP_COMMA_IF(N) \
                                      BOOST_PP_ENUM_PARAMS(N, Param)) \
                                      BOOST_PP_COMMA_IF(N) \
                                      BOOST_PP_ENUM_PARAMS(N, ParameterInfo i)) { \
  constructor_info f(reflections::type_info_handler \
      <TypeInfo, instance (*)(ParamFirst BOOST_PP_COMMA_IF(N) \
                              BOOST_PP_ENUM_PARAMS(N, Param))> \
        ::get_class_type()); \
  BOOST_REFLECTION_PUSH_PARAMETER_INFO(f, N);  \
  reflection_->constructors_.insert(std::make_pair<TypeInfo, FunctionPtr>( \
      f, reinterpret_cast<FunctionPtr>(func))); \
}



// Free functions
#define BOOST_REFLECTION_REFLECTOR_CONSTRUCT_FUNCTION(Z, N, _) \
template <class T BOOST_PP_COMMA_IF(N) \
          BOOST_PP_ENUM_PARAMS(N, class Param)> \
instance construct_instance(BOOST_PP_ENUM_BINARY_PARAMS(N, Param, p)) { \
  return instance(static_cast<void*>( \
    construct<T BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, Param)> \
     (BOOST_PP_ENUM_PARAMS(N, p))), \
                  &destruct<T>); \
}
#define BOOST_REFLECTION_REFLECTOR_CALL_MEMBER_FUNCTION(Z, N, _) \
template <class T, class ReturnValue BOOST_PP_COMMA_IF(N) \
          BOOST_PP_ENUM_PARAMS(N, class Param)> \
static ReturnValue call_member(void * val, \
                               MemberFunctionPtr member_function \
                               BOOST_PP_COMMA_IF(N) \
                               BOOST_PP_ENUM_BINARY_PARAMS(N, Param, p)) { \
  T * actual = static_cast<T*>(val); \
  ReturnValue (T::*func)(BOOST_PP_ENUM_PARAMS(N, Param)) =  \
    reinterpret_cast<ReturnValue (T::*)(BOOST_PP_ENUM_PARAMS(N, Param))> \
      (member_function); \
  return (actual->*func)(BOOST_PP_ENUM_PARAMS(N, p)); \
}
template <class T>
void destruct(void * val) {
  delete static_cast<T*>(val);
}
BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS), \
                BOOST_REFLECTION_REFLECTOR_CONSTRUCT_FUNCTION, _)
BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS), \
                BOOST_REFLECTION_REFLECTOR_CALL_MEMBER_FUNCTION, _)



template <class T, class Info = std::string, class ParameterInfo = void,
          class TypeInfo = reflections::default_type_info>
class reflector
{
public:
  //  Initialize with a pointer to the reflection
  //  this reflector will be reflecting into
  reflector(basic_reflection<Info, ParameterInfo, TypeInfo>
            * current_reflection)
  : reflection_(current_reflection) {
  }
  typedef basic_function_info<Info, TypeInfo, ParameterInfo> function_info;
  typedef basic_constructor_info<TypeInfo, ParameterInfo> constructor_info;
  void reflect_constructor() {
    add_constructor(&construct_instance<T>);
  }
  BOOST_PP_REPEAT(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS, \
                  BOOST_REFLECTION_REFLECTOR_REFLECT_CONSTRUCTOR_INFO_FUNCTION, _)
 /* template <class ReturnValue>
  void reflect(ReturnValue (T::*func)(), Info info) {
    function_info f(reflections::type_info_handler<TypeInfo,
                    ReturnValue (*)()>::get_class_type(), info);
    ReturnValue (*f2)(void *, MemberFunctionPtr) = &call_member<ReturnValue>;
    std::pair<MemberFunctionPtr, FunctionPtr> 
      p(reinterpret_cast<MemberFunctionPtr>(func),
        reinterpret_cast<FunctionPtr>(f2));
    std::pair<function_info, std::pair<MemberFunctionPtr, FunctionPtr> >
      p2(f, p);
    reflection_->functions_.insert(p2);
  }*/
  BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS), \
    BOOST_REFLECTION_REFLECTOR_REFLECT_INFO_FUNCTION, _)
  void reflect(void (T::*func)(), Info info) {
    function_info f(reflections::type_info_handler<TypeInfo,
                      void (*)()>::get_class_type(), info);
    void (*f2)(void *, MemberFunctionPtr) = &call_member<T, void>; \
    std::pair<MemberFunctionPtr, FunctionPtr>
      in_pair(reinterpret_cast<MemberFunctionPtr>(func),
        reinterpret_cast<FunctionPtr>(f2));
    std::pair<function_info, std::pair<MemberFunctionPtr, FunctionPtr> >
      out_pair(f, in_pair);
    reflection_->functions_.insert(out_pair);
  }
  BOOST_PP_REPEAT(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS, \
    BOOST_REFLECTION_REFLECTOR_REFLECT_INFO_VOID_RET_FUNCTION, _)
private:
  void add_constructor(instance (*func)()) {
    reflection_->constructors_.insert(std::make_pair<TypeInfo, FunctionPtr>( 
      reflections::type_info_handler<TypeInfo, instance (*)()>::get_class_type(),
      reinterpret_cast<FunctionPtr>(func)));
  }
  BOOST_PP_REPEAT(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS, \
    BOOST_REFLECTION_REFLECTOR_ADD_CONSTRUCTOR_INFO_FUNCTION, _)
  basic_reflection<Info, ParameterInfo, TypeInfo> * reflection_;
};

template <class T, class Info, class TypeInfo>
class reflector<T, Info, void, TypeInfo>
{
public:
  //  Initialize with a pointer to the reflection
  //  this reflector will be reflecting into
  reflector(basic_reflection<Info, void, TypeInfo> * current_reflection)
  : reflection_(current_reflection) {
  }
  typedef basic_function_info<Info, TypeInfo> function_info;
  typedef basic_constructor_info<Info, TypeInfo> constructor_info;
  void reflect_constructor() {
    add_constructor(&construct_instance<T>);
  }
  BOOST_PP_REPEAT(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS, \
                  BOOST_REFLECTION_REFLECTOR_REFLECT_CONSTRUCTOR_FUNCTION, _)
    /* template <class ReturnValue>
    void reflect(ReturnValue (T::*func)(), Info info) {
      function_info f(reflections::type_info_handler<TypeInfo,
                      ReturnValue (*)()>::get_class_type(), info);
      ReturnValue (*f2)(void *, MemberFunctionPtr) = &call_member<ReturnValue>;
      std::pair<MemberFunctionPtr, FunctionPtr> 
        p(reinterpret_cast<MemberFunctionPtr>(func),
          reinterpret_cast<FunctionPtr>(f2));
      std::pair<function_info, std::pair<MemberFunctionPtr, FunctionPtr> >
        p2(f, p);
      reflection_->functions_.insert(p2);
    }*/
    BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS), \
                    BOOST_REFLECTION_REFLECTOR_REFLECT_FUNCTION, _)
private:
    void add_constructor(instance (*func)()) {
      reflection_->constructors_.insert(std::make_pair<TypeInfo, FunctionPtr>( 
        reflections::type_info_handler<TypeInfo, instance (*)()>
        ::get_class_type(), reinterpret_cast<FunctionPtr>(func)));
    }
  BOOST_PP_REPEAT(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS, \
                  BOOST_REFLECTION_REFLECTOR_ADD_CONSTRUCTOR_FUNCTION, _)
    basic_reflection<Info, void, TypeInfo> * reflection_;
};

#undef BOOST_REFLECTION_REFLECTOR_REFLECT_CONSTRUCTOR_FUNCTION
}}
#endif
