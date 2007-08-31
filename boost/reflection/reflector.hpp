/*
 * Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_EXTENSION_REFLECTOR_HPP
#define BOOST_EXTENSION_REFLECTOR_HPP

#include <boost/reflection/factory.hpp>
#include <boost/reflection/function.hpp>
#include <boost/reflection/constructor.hpp>
#include <boost/reflection/instance.hpp>
#include <boost/reflection/reflection.hpp>
namespace boost {namespace reflections {


  
/*  template <class Info = std::string,
  class TypeInfo = extensions::default_type_info>
  class generic_reflector
{
public:
  typedef basic_function_info<Info, TypeInfo> function_info;
  virtual ~generic_reflector() {}
  
protected:
  
};*/
template <class T, class Info = std::string,
          class TypeInfo = extensions::default_type_info>
class reflector
{
public:
  //  Initialize with a pointer to the reflection
  //  this reflector will be reflecting into
  reflector(basic_reflection<Info, TypeInfo> * current_reflection)
  : reflection_(current_reflection) {
  }
  typedef basic_function_info<Info, TypeInfo> function_info;
  void reflect_constructor() {
    add_constructor(&construct);
  }
  template <class ReturnValue>
  void reflect(ReturnValue (T::*func)(), Info info) {
    function_info f(extensions::type_info_handler<TypeInfo,
                    ReturnValue (*)()>::get_class_type(), info);
    ReturnValue (*f2)(void *, MemberFunctionPtr) = &call_member<ReturnValue>;
    std::pair<MemberFunctionPtr, FunctionPtr> 
      p(reinterpret_cast<MemberFunctionPtr>(func),
        reinterpret_cast<FunctionPtr>(f2));
    std::pair<function_info, std::pair<MemberFunctionPtr, FunctionPtr> >
      p2(f, p);
    reflection_->functions_.insert(p2);
  }
private:
  void add_constructor(instance (*func)()) {
    reflection_->constructors_.insert(std::make_pair<TypeInfo, FunctionPtr>( 
      extensions::type_info_handler<TypeInfo, instance (*)()>::get_class_type(),
      reinterpret_cast<FunctionPtr>(func)));
  }
  static instance construct() {
    return instance(static_cast<void*>(new T()), &destruct); 
  }
  template <class ReturnValue>
  static ReturnValue call_member(void * val,
                                 MemberFunctionPtr member_function) {
    T * actual = static_cast<T*>(val);
    ReturnValue (T::*func)() = 
      reinterpret_cast<ReturnValue (T::*)()>(member_function);
    return (actual->*func)();
  }
  static void destruct(void * val) {
    delete static_cast<T*>(val); 
  }
  basic_reflection<Info, TypeInfo> * reflection_;
};
  /*
template <class TypeInfo, class Info>
class function_info
{
public:
  TypeInfo type_info_;
  Info info_;
  bool operator<(function_info & second) {
    return type_info_ < second.type_info_ ||
      (type_info_ == second.type_info_ &&
       info_ < second.type_info_);
  }
};
template <class Info = std::string,
          class TypeInfo = extensions::default_type_info>
class generic_reflector
{
public:
  
};
template <class T, class Info = std::string,
          class TypeInfo = extensions::default_type_info>
class reflector : public generic_reflector<Info, TypeInfo>
{
public:
  void reflect_constructor() {
    constructors_.insert(std::pair<TypeInfo, generic_constructor*>
                         (type_info_handler::get_class_type<TypeInfo, constructor>,
                          new constructor()));
  }
  template <class ReturnValue>
  void reflect(ReturnValue (T::*func)(), Info info) {
    functions_.insert(std::pair<std::pair<TypeInfo, Info>, generic_function*>
                      (std::pair<TypeInfo, Info>
                       (type_info_handler::get_class_type<function<ReturnValue> >, info),
                       (new function<ReturnValue>(func))));
  }
  ~reflector() {
    for (ConstructorIterator it = contructors_.begin();
         it != constructors_.end(); ++it) {
      delete it->second;
    }
    for (FactoryIterator it = factories_.begin();
         it != factories_.end(); ++it) {
      delete it->second;
    }
    for (FunctionIterator it = functions_.begin();
         it != functions_.end(); ++it) {
      delete it->second;
    }
  }
private:
  typedef std::map<TypeInfo, generic_constructor<T>*>::iterator
  ConstructorIterator;
  typedef std::map<function_info<TypeInfo, Info>, 
    generic_constructor<T>*>::iterator FactoryIterator;
  typedef std::map<function_info<TypeInfo, Info>,
    generic_function<T>*>::iterator FunctionIterator;
  std::map<TypeInfo, generic_constructor<T>*> constructors_;
  std::map<function_info<TypeInfo, Info>, generic_constructor<T>*> factories_;
  std::map<function_info<TypeInfo, Info>, generic_function<T>*> functions_;
};*/
}}
#endif