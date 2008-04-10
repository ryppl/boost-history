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

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/reflection/common.hpp>
#include <boost/reflection/factory.hpp>
#include <boost/reflection/function.hpp>
#include <boost/reflection/constructor.hpp>
#include <boost/reflection/instance.hpp>
#include <boost/reflection/reflection.hpp>

namespace boost {namespace reflections {
using extensions::type_info_handler;
namespace impl {
template <class T>
void destruct(void * val) {
  delete static_cast<T*>(val);
}

#define BOOST_PP_ITERATION_LIMITS (0, \
    BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS) - 1)
#define BOOST_PP_FILENAME_1 <boost/reflection/impl/reflector_free_functions.hpp>
#include BOOST_PP_ITERATE()

}  // namespace impl

template <class T, class Info = std::string, class ParameterInfo = void,
          class TypeInfo = extensions::default_type_info>
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
    add_constructor(&impl::construct_instance<T>);
  }
#define BOOST_PP_ITERATION_LIMITS (0, \
    BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS) - 1)
#define BOOST_PP_FILENAME_1 <boost/reflection/impl/reflector_parameter_functions.hpp>
#include BOOST_PP_ITERATE()
  void reflect(void (T::*func)(), Info info) {
    function_info f(reflections::type_info_handler<TypeInfo,
                      void (*)()>::get_class_type(), info);
    void (*f2)(void *, MemberFunctionPtr) = &impl::call_member<T, void>; \
    std::pair<MemberFunctionPtr, FunctionPtr>
      in_pair(reinterpret_cast<MemberFunctionPtr>(func),
        reinterpret_cast<FunctionPtr>(f2));
    std::pair<function_info, std::pair<MemberFunctionPtr, FunctionPtr> >
      out_pair(f, in_pair);
    reflection_->functions_.insert(out_pair);
  }
private:
  void add_constructor(instance (*func)()) {
    reflection_->constructors_.insert(std::make_pair<TypeInfo, FunctionPtr>( 
      reflections::type_info_handler<TypeInfo, instance (*)()>::get_class_type(),
      reinterpret_cast<FunctionPtr>(func)));
  }
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
    add_constructor(&impl::construct_instance<T>);
  }
#define BOOST_PP_ITERATION_LIMITS (0, \
    BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS) - 1)
#define BOOST_PP_FILENAME_1 <boost/reflection/impl/reflector_functions.hpp>
#include BOOST_PP_ITERATE()
private:
    void add_constructor(instance (*func)()) {
      reflection_->constructors_.insert(std::make_pair<TypeInfo, FunctionPtr>( 
        reflections::type_info_handler<TypeInfo, instance (*)()>
        ::get_class_type(), reinterpret_cast<FunctionPtr>(func)));
    }
    basic_reflection<Info, void, TypeInfo> * reflection_;
};
}}
#endif
