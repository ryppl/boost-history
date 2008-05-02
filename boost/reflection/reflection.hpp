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
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/reflection/constructor.hpp>
#include <boost/reflection/constructor_info.hpp>
#include <boost/reflection/data.hpp>
#include <boost/reflection/data_info.hpp>
#include <boost/reflection/factory.hpp>
#include <boost/reflection/function.hpp>
#include <boost/reflection/function_info.hpp>

namespace boost {
namespace reflections {
using extensions::type_info_handler;
namespace impl {
template <class T>
void destruct(void * val) {
  delete static_cast<T*>(val);
}

#define BOOST_PP_ITERATION_LIMITS (0, \
    BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS) - 1)
#define BOOST_PP_FILENAME_1 \
  <boost/reflection/impl/reflector_free_functions.hpp>
#include BOOST_PP_ITERATE()

// This is used to store, indexed by type information,
// any normal function pointers.
typedef void (*FunctionPtr)();

}  // namespace impl


// By default, ParameterInfo is not used. Note that using adapters
// requires ParameterInfo.
template <class Info = std::string, class ParameterInfo = void,
          class TypeInfo = extensions::default_type_info>
class basic_reflection {
public:
  // A reflector is used to add functions and constructors to
  // a reflected class.
  
  template <class T>
  class reflector
  {
  public:
    //  Initialize with a pointer to the reflection
    //  this reflector will be reflecting into
    reflector(basic_reflection<Info, ParameterInfo, TypeInfo>*
              current_reflection)
      : reflection_(current_reflection) {
    }
  
    // Typedefs for the specific instantiations used by this class.
    typedef basic_function_info<Info, TypeInfo, ParameterInfo> function_info;
    typedef basic_constructor_info<TypeInfo, ParameterInfo> constructor_info;
  
    reflector& constructor() {
      instance (*ctor_func)()(&impl::construct_instance<T>);
      reflection_->constructors_.insert(std::make_pair<TypeInfo, impl::FunctionPtr>(
          reflections::type_info_handler<TypeInfo, instance (*)()>
          ::get_class_type(), reinterpret_cast<impl::FunctionPtr>(ctor_func)));
      return *this;
    }

    template <class S>
    reflector& data(S T::*data_ptr, Info info) {
      data_info f(reflections::type_info_handler<TypeInfo, S>
                    ::get_class_type(), info);

      std::pair<impl::MemberPtr, impl::FunctionPtr>
        p(reinterpret_cast<impl::MemberPtr>(data_ptr),
          reinterpret_cast<impl::FunctionPtr>(&impl::get_data_from_ptr<T, S>));
      std::pair<data_info, std::pair<impl::MemberPtr, impl::FunctionPtr> >
        p2(f, p);
      reflection_->data_.insert(p2);
      return *this;
    }
  #define BOOST_PP_ITERATION_LIMITS (0, \
      BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS) - 1)
  #define BOOST_PP_FILENAME_1 \
    <boost/reflection/impl/reflector_parameter_functions.hpp>
  #include BOOST_PP_ITERATE()
    reflector& function(void (T::*func)(), Info info) {
      function_info f(reflections::type_info_handler<TypeInfo,
                        void (*)()>::get_class_type(), info, false);
      void (*f2)(void *, impl::MemberFunctionPtr) = &impl::call_member<T, void>;
      std::pair<impl::MemberFunctionPtr, impl::FunctionPtr>
        in_pair(reinterpret_cast<impl::MemberFunctionPtr>(func),
          reinterpret_cast<impl::FunctionPtr>(f2));
      std::pair<function_info,
                std::pair<impl::MemberFunctionPtr,
                          impl::FunctionPtr> >
        out_pair(f, in_pair);
      reflection_->functions_.insert(out_pair);
      return *this;
    }
  private:
    basic_reflection<Info, ParameterInfo, TypeInfo>* reflection_;
  };
#define BOOST_PP_ITERATION_LIMITS (0, \
    BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS) - 1)
#define BOOST_PP_FILENAME_1 <boost/reflection/impl/reflection.hpp>
#include BOOST_PP_ITERATE()
  instance_constructor<> get_constructor() {
    constructor_info t(reflections::type_info_handler<TypeInfo,
    instance (*)()>::get_class_type());
    typename std::map<constructor_info, impl::FunctionPtr>::iterator it =
      constructors_.find(t);
    if (it == constructors_.end()) {
      return instance_constructor<>();
    } else {
      return reinterpret_cast<instance (*)()>(it->second);
    }
  }

  template <class T>
  data<T> get_data(Info info) {
    // Construct a data_info structure to look up the function in the map.
    data_info d(reflections::type_info_handler<TypeInfo, T>
                            ::get_class_type(), info);
  
    // Look up the function.
    typename std::map<data_info,
      std::pair<impl::MemberPtr, impl::FunctionPtr> >::iterator it =
      data_.find(d);
  
    if (it == data_.end()) {
      // If it does not exist, return an empty function object.
      return data<T>();
    } else {
      return data<T>
        // reinterpret_cast is safe, because we looked it up by its type.
        (it->second.first,
         reinterpret_cast<T& (*)(void*, impl::MemberPtr)>
           (it->second.second));
    }
  }

  template <class T>
  reflector<T> reflect() {
    return reflector<T>(this);
  }
private:
  typedef basic_function_info<Info, TypeInfo, ParameterInfo> function_info;
  typedef basic_constructor_info<TypeInfo, ParameterInfo> constructor_info;
  typedef basic_data_info<Info, TypeInfo> data_info;
  std::map<constructor_info, impl::FunctionPtr> constructors_;
  std::map<function_info,
    std::pair<impl::MemberFunctionPtr, impl::FunctionPtr> > functions_;
  std::map<data_info,
           std::pair<impl::MemberPtr, impl::FunctionPtr> > data_;
};
template <class Info, class TypeInfo>
class basic_reflection<Info, void, TypeInfo>
{
public:
  template <class T>
  class reflector
  {
  public:
    //  Initialize with a pointer to the reflection
    //  this reflector will be reflecting into
    reflector(basic_reflection<Info, void, TypeInfo> * current_reflection)
    : reflection_(current_reflection) {
    }
  
    // Typedefs for the specific instantiations used by this class.
    typedef basic_function_info<Info, TypeInfo> function_info;
    typedef basic_constructor_info<Info, TypeInfo> constructor_info;
  
    // Reflect the default constructor (other constructors in
    // impl/reflector_functions.hpp).
    reflector& constructor() {
      instance (*ctor_func)()(&impl::construct_instance<T>);
      reflection_->constructors_.insert(std::make_pair<TypeInfo, impl::FunctionPtr>(
          reflections::type_info_handler<TypeInfo, instance (*)()>
          ::get_class_type(), reinterpret_cast<impl::FunctionPtr>(ctor_func)));
      return *this;
    }

    template <class S>
    reflector& data(S T::*data_ptr, Info info) {
      data_info f(reflections::type_info_handler<TypeInfo, S>
                    ::get_class_type(), info);
      S& (*func)(void*, impl::MemberPtr) = &impl::get_data_from_ptr<T, S>;
      std::pair<impl::MemberPtr, impl::FunctionPtr>
        p(reinterpret_cast<impl::MemberPtr>(data_ptr),
          reinterpret_cast<impl::FunctionPtr>(func));
      std::pair<data_info, std::pair<impl::MemberPtr, impl::FunctionPtr> >
        p2(f, p);
      reflection_->data_.insert(p2);
      return *this;
    }
  #define BOOST_PP_ITERATION_LIMITS (0, \
      BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS) - 1)
  #define BOOST_PP_FILENAME_1 \
    <boost/reflection/impl/reflector_functions.hpp>
  #include BOOST_PP_ITERATE()
  private:
    basic_reflection<Info, void, TypeInfo> * reflection_;
  };

#define BOOST_PP_ITERATION_LIMITS (0, \
    BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS) - 1)
#define BOOST_PP_FILENAME_1 <boost/reflection/impl/reflection.hpp>
#include BOOST_PP_ITERATE()
  instance_constructor<> get_constructor() {
    constructor_info t(reflections::type_info_handler<TypeInfo,
    instance (*)()>::get_class_type());
    typename std::map<constructor_info, impl::FunctionPtr>::iterator it =
      constructors_.find(t);
    if (it == constructors_.end()) {
      return instance_constructor<>();
    } else {
      return reinterpret_cast<instance (*)()>(it->second);
    }
  }

  template <class T>
  data<T> get_data(Info info) {
    // Construct a data_info structure to look up the function in the map.
    data_info d(reflections::type_info_handler<TypeInfo, T>
                            ::get_class_type(), info);
  
    // Look up the function.
    typename std::map<data_info,
      std::pair<impl::MemberPtr, impl::FunctionPtr> >::iterator it =
      data_.find(d);
  
    if (it == data_.end()) {
      // If it does not exist, return an empty function object.
      return data<T>();
    }
    return data<T>
      // reinterpret_cast is safe, because we looked it up by its type.
      (it->second.first,
       reinterpret_cast<T& (*)(void*, impl::MemberPtr)>
         (it->second.second));
  }

  template <class T>
  reflector<T> reflect() {
    return reflector<T>(this);
  }

private:
  typedef basic_function_info<Info, TypeInfo> function_info;
  typedef basic_constructor_info<TypeInfo> constructor_info;
  typedef basic_data_info<Info, TypeInfo> data_info;
  std::map<constructor_info, impl::FunctionPtr> constructors_;
  std::map<function_info,
           std::pair<impl::MemberFunctionPtr, impl::FunctionPtr> > functions_;
  std::map<data_info,
           std::pair<impl::MemberPtr, impl::FunctionPtr> > data_;
};

typedef basic_reflection<> reflection;
}}
#endif  // BOOST_REFLECTION_REFLECTION_HPP
