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
#include <boost/reflection/factory.hpp>
#include <boost/reflection/function.hpp>
#include <boost/reflection/function_info.hpp>

namespace boost {
namespace reflections {
using extensions::type_info_handler;
typedef void (*FunctionPtr)();

// The basic_function_info class is used as a key in the map
// of functions available for the current reflection.
// There are two types - those with ParameterInfo defined, and
// those without.
template<class Info, class TypeInfo, class ParameterInfo = void>
struct basic_function_info {
  // The type of the function pointer in the map.
  TypeInfo type_info_;
  // A description of the function pointer.
  Info info_;

  // A description for each parameter of the function.
  // If ParameterInfo=void, this does not appear.
  std::vector<ParameterInfo> parameter_info_;

  // Constructors
  basic_function_info(TypeInfo t, Info i) : type_info_(t), info_(i) {
  }

  basic_function_info(const basic_function_info & s) 
    : type_info_(s.type_info_), info_(s.info_) {
  }

  basic_function_info & operator=(basic_function_info & s) {
    type_info_ = s.type_info_;
    info_ = s.info_;
  }

  // Less-than operator - for maps.
  friend inline bool operator<(const basic_function_info & t,
                               const basic_function_info & s) {
    return t.type_info_ < s.type_info_ ||
    (t.type_info_ == s.type_info_ &&
     t.info_ < s.info_);
  }
};

// Same as the above, but without ParameterInfo.
template<class Info, class TypeInfo>
struct basic_function_info<Info, TypeInfo> {
  // The type of the function pointer in the map.
  TypeInfo type_info_;
  // A description of the function pointer.
  Info info_;

  // Constructors.
  basic_function_info(TypeInfo t, Info i) : type_info_(t), info_(i) {
  }

  basic_function_info(const basic_function_info & s) 
    : type_info_(s.type_info_), info_(s.info_) {
  }

  basic_function_info & operator=(basic_function_info & s) {
    type_info_ = s.type_info_;
    info_ = s.info_;
  }

  // Less-than operator - for maps.
  friend inline bool operator<(const basic_function_info & t,
                               const basic_function_info & s) {
    return t.type_info_ < s.type_info_ ||
    (t.type_info_ == s.type_info_ &&
     t.info_ < s.info_);
  }
};

// The basic_constructor_info class is used as a key in the map
// of constructors available for the current reflection.
// There are two types - those with ParameterInfo defined, and
// those without.
template<class TypeInfo, class ParameterInfo = void>
struct basic_constructor_info {
  // The type of the function pointer used to construct
  // the object this constructor_info is for.
  TypeInfo type_info_;

  // A description for each parameter of the function.
  // If ParameterInfo=void, this does not appear.
  std::vector<ParameterInfo> parameter_info_;

  // Constructors.
  basic_constructor_info(TypeInfo t) : type_info_(t) {
  }

  basic_constructor_info(const basic_constructor_info & s) 
    : type_info_(s.type_info_) {
  }

  basic_constructor_info & operator=(basic_constructor_info & s) {
    type_info_ = s.type_info_;
  }

  // Less than operator - for maps.
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
#define BOOST_PP_ITERATION_LIMITS (0, \
    BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS) - 1)
#define BOOST_PP_FILENAME_1 <boost/reflection/impl/reflection.hpp>
#include BOOST_PP_ITERATE()
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
#define BOOST_PP_ITERATION_LIMITS (0, \
    BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS) - 1)
#define BOOST_PP_FILENAME_1 <boost/reflection/impl/reflection.hpp>
#include BOOST_PP_ITERATE()
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
