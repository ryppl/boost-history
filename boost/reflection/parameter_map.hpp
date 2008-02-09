/*
 * Boost.Reflection / paramater map (store parameter information for calls)
 *
 * (C) Copyright Mariano G. Consoni 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */


#ifndef BOOST_REFLECTION_PARAMETER_MAP_HPP
#define BOOST_REFLECTION_PARAMETER_MAP_HPP
#include <boost/reflection/typeinfo.hpp>
#include <exception>
#include <multimap>
#include <vector>
namespace boost { namespace reflections {
class param_not_found_exception : public std::exception {
public:
  virtual const char* what() {
    return "Cannot convert types";
  }
};
typedef void (*FunctionPtr)();
template <class TypeInfo>
class generic_parameter {
public:
  virtual ~basic_parameter() {}
  virtual TypeInfo type() = 0;
  template <class T>
  bool can_cast() {
    type_info i = type_info_handler<TypeInfo, T>::get_class_type();
    for (vector_type::const_iterator it = converter_.begin();
         it != converters_.end(); ++it) {
      if (it->first == i)
        return true;
    }
  }
  template <class T>
  T* cast() {
    type_info i = type_info_handler<TypeInfo, T>::get_class_type();
    for (vector_type::const_iterator it = converter_.begin();
         it != converters_.end(); ++it) {
      if (it->first == i)
        return true;
    }
  }
  tempate <class T>
  T cast() {
    type_info i = type_info_handler<TypeInfo, T>::get_class_type();
    for (vector_type::const_iterator it = converter_.begin();
         it != converters_.end(); ++it) {
      if (it->first == i) {
        T ret;
        cast_and_store(it, &ret);
      } 
    }
    throw param_not_found_exception();
  }
protected
  void cast_and_store(vector_type::iterator it, void* dest);
private:
  typedef std::vector<std::pair<TypeInfo, FunctionPtr> > vector_type;
  vector_type converters_;
};

template <class S, class T>
S convert(T val) {
  return static_cast<S>(T); 
}



template <class T, class TypeInfo = reflections::default_type_info>
class parameter : public generic_parameter<TypeInfo> {
public:
  template <class A, B, C>
  friend class parameter_map;
  
  parameter(T value) : value_(value) {
    // Add converter for current type.
    converters_.push_back
      (make_pair(reflections::type_info_handler<TypeInfo, T>::get_class_type(),
                 reinterpret_cast<FunctionPtr>(&convert<T, T&>)));
  }
  template <class S>
  void converts_to(S (*convert_func)(T) = &convert<S, T>) {
    converters_.push_back
      (make_pair(reflections::type_info_handler<TypeInfo, S>::get_class_type(),
                 reinterpret_cast<FunctionPtr>(convert_func)));
  }
  template <class S>
  void ref_converts_to(S (*convert_func)(T&) = &convert<S, T>) {
    converters_.push_back
      (make_pair(reflections::type_info_handler<TypeInfo, S>::get_class_type(),
                 reinterpret_cast<FunctionPtr>(convert_func)));
  }
private:
  T value_;
};

class parameter_unavailable_exception : public std::exception {
public:
  virtual const char * what() {
    return "Type not found in parameter_map";
  }
};

template <class Info = std::string,
          class TypeInfo = default_type_info>
class basic_parameter_map 
  : public std::multimap<Info, basic_parameter*> {
public:
  ~basic_parameter_map() {
    for (iterator it = begin(); it != end(); ++it) {
      delete it->second;
    }
  }
  typedef std::multimap<Info, basic_parameter*> map_type;
  using map_type::equal_range;
  using map_type::begin;
  using map_type::end;
  typedef MapType::iterator iterator;
  template <class T>
  T get(Info info) {
    TypeInfo current_type =
      reflections::type_info_handler<TypeInfo, T>::get_class_type();
    std::pair<iterator, iterator> its = equal_range(info);
    for (iterator current = its->first; current != its->second; ++current) {
      
      for (current->second->type() == current_type) {
         
      }
    }
  }
private:
 
};
typedef basic_paramter_map<> parameter_map;


}}

#endif // BOOST_REFLECTION_PARAMETER_MAP_HPP
