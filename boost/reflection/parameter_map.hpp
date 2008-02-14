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
#include <boost/extension/impl/typeinfo.hpp>
#include <exception>
#include <map>
#include <vector>
namespace boost { namespace reflections {
using extensions::type_info_handler;

class conversion_not_found_exception : public std::exception {
public:
  virtual const char* what() {
    return "Cannot convert types";
  }
};
typedef void (*FunctionPtr)();
template <class TypeInfo = extensions::default_type_info>
class generic_parameter {
public:
  virtual ~generic_parameter() {
    for (typename std::map<TypeInfo, basic_converter*>::iterator it = converters_.begin();
         it != converters_.end(); ++it) {
      delete it->second;
    }
  }
  virtual TypeInfo type() = 0;
  template <class T>
  bool can_cast() {
    TypeInfo i = type_info_handler<TypeInfo, T>::get_class_type();
    return (converters_.find(i) != converters_.end());
  }/*
  template <class T>
  T* cast() {
    TypeInfo i = type_info_handler<TypeInfo, T>::get_class_type();
    for (typename vector_type::const_iterator it = converter_.begin();
         it != converters_.end(); ++it) {
      if (it->first == i)
        return true;
    }
  }*/
  template <class S>
  void cast(S* dest) {
    TypeInfo i = type_info_handler<TypeInfo, S>::get_class_type();
    typename std::map<TypeInfo, basic_converter*>::iterator it = converters_.find(i);
    if (it != converters_.end()) {
      it->second->convert(value_, reinterpret_cast<void*>(dest));
      return;
    }
    throw conversion_not_found_exception();
  }
protected:
  generic_parameter(void* value) : value_(value) {
  }
  class basic_converter {
  public:
    virtual void convert(void* src, void* dest) = 0;
    virtual ~basic_converter() {};
  };
 // typedef std::vector<std::pair<TypeInfo, FunctionPtr> > vector_type;
 // vector_type converters_;
  std::map<TypeInfo, basic_converter*> converters_;
private:
  void* value_;
};

template <class T, class TypeInfo = extensions::default_type_info>
class parameter : public generic_parameter<TypeInfo> {
public:
  template <class A, class B>
  friend class basic_parameter_map;
  
  virtual TypeInfo type() {
    return reflections::type_info_handler<TypeInfo, T>::get_class_type();
  }
  parameter(T value)
    : generic_parameter<TypeInfo>(reinterpret_cast<void*>(&value_)),
      value_(value) {
    // Add converter for current type.
    generic_parameter<TypeInfo>::converters_.insert
      (std::make_pair(reflections::type_info_handler<TypeInfo, T>::get_class_type(),
                 new default_converter<T>()));
  }
  template <class S>
  void converts_to_with_func(void (*convert_func)(T*, S*)) {
    generic_parameter<TypeInfo>::converters_.insert
      (make_pair(reflections::type_info_handler<TypeInfo, S>::get_class_type(),
                 new specialized_converter<S>(convert_func)));
  }
  template <class S>
  void converts_to() {
    generic_parameter<TypeInfo>::converters_.push_back
      (make_pair(reflections::type_info_handler<TypeInfo, S>::get_class_type(),
                 new default_converter<S>()));
  }
private:
  
  template <class S>
  class default_converter : public generic_parameter<TypeInfo>::basic_converter {
  public:
    virtual void convert(void* val, void* dest) {
      S* s = reinterpret_cast<S*>(dest);
      *s = static_cast<S>(*reinterpret_cast<T*>(val));
    }
  };
  template <class S>
  class specialized_converter : public generic_parameter<TypeInfo>::basic_converter {
  public:
    specialized_converter(void (*convert_function)(T*, S*))
      : convert_function_(convert_function) {
    }
    virtual void convert(TypeInfo type_info, T* val, void* dest) {
      S* s = reinterpret_cast<S*>(dest);
      (*convert_function_)(reinterpret_cast<T*>(val), s);
    }
  private:
    void (*convert_function_)(T*, S*);
  };
  T value_;
};

class parameter_unavailable_exception : public std::exception {
public:
  virtual const char * what() {
    return "Type not found in parameter_map";
  }
};

template <class Info = std::string,
          class TypeInfo = extensions::default_type_info>
class basic_parameter_map 
  : public std::multimap<Info, generic_parameter<TypeInfo>*> {
public:
  ~basic_parameter_map() {
    for (typename map_type::iterator it = begin(); it != end(); ++it) {
      delete it->second;
    }
  }
  typedef std::multimap<Info, generic_parameter<TypeInfo>*> map_type;
  using map_type::equal_range;
  using map_type::begin;
  using map_type::end;
  template <class D>
  std::vector<generic_parameter<TypeInfo>*> get(Info info) {
    std::vector<generic_parameter<TypeInfo>*> parameters;
    std::pair<typename map_type::iterator, typename map_type::iterator> its = equal_range(info);
    for (typename map_type::iterator current = its->first; current != its->second; ++current) {
      generic_parameter<TypeInfo>& p = *current->second;
      if (p.template can_cast<D>()) {
        parameters.push_back(current->second);
      }
    }
    return parameters;
  }
};
typedef basic_parameter_map<> parameter_map;


}}

#endif // BOOST_REFLECTION_PARAMETER_MAP_HPP
