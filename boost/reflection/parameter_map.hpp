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
#include <multimap>
#include <vector>
namespace boost { namespace reflections {
  
typedef void (*FunctionPtr)();
template <class TypeInfo>
class generic_parameter {
public:
  virtual ~basic_parameter() {}
  virtual TypeInfo type() = 0;
};

template <class S, class T>
S convert(generic_parameter param) {
  T val = 
  return static_cast<S>(val); 
}



template <class T, class TypeInfo = reflections::default_type_info>
class parameter : public generic_parameter<TypeInfo> {
public:
  template <class A, B, C>
  friend class parameter_map;
  parameter(T value) : value_(value) {}
  template <class S>
  void converts_to(S (*convert_func)(T) = &convert<S, T>) {
    converters_.push_back
      (make_pair(reflections::type_info_handler<TypeInfo, S>::get_class_type(),
                 reinterpret_cast<FunctionPtr>(convert_func)));
  }
private:
  typedef std::vector<std::pair<TypeInfo, FunctionPtr> > vector_type;
  vector_type converters_;
  T value_;
};

class parameter_unavailable_exception : public std::exception {
public:
  virtual const char * what() {
    return "Type not found in parameter_map";
  }
};

template <class Info = std::string,
          class TypeInfo = reflections::default_type_info>
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
