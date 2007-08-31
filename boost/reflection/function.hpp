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
template <class ReturnValue>
class function {
public:
  function(ReturnValue (*func)(void *, MemberFunctionPtr) = 0,
           MemberFunctionPtr member_function = 0) 
  : func_(func),
    member_function_(member_function) {
  }
  ReturnValue call(instance & inst) {
    return (*func_)(inst.val_, member_function_);
  }
  ReturnValue operator()(instance & inst) {
    return (*func_)(inst.val_, member_function_); 
  }
  bool valid() {
    return member_function_ != 0 && func_ != 0;
  }
private:
  ReturnValue (*func_)(void *, MemberFunctionPtr);
  MemberFunctionPtr member_function_;
};
/*template <class T>
class generic_function {
  virtual ~generic_function() {}
  virtual void call(void ** params) = 0;
};
template <class T, class ReturnValue = void>
class function : public generic_function<T> {
public:
  function(ReturnValue (T::*func)())
  : func_(func) {}
  virtual void call(T * obj, void ** params) {
    *static_cast<ReturnValue*>(params[0]) =
    (obj->*func_)();
  }
private:
  ReturnValue (T::*func_)();
};
template <class T>
class function<T, void> : public generic_function<T> {
public:
  function(void (T::*func)())
  : func_(func) {}
  virtual void call(T * obj, void ** params) {
    (obj->*func_)();
  }
private:
  void (T::*func_)();
};  */
}}
#endif