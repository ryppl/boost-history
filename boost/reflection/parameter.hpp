/*
 * Boost.Reflection / paramater map (store parameter information for calls)
 *
 * (C) Copyright Mariano G. Consoni 2008
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */


#ifndef BOOST_REFLECTION_PARAMETER_HPP
#define BOOST_REFLECTION_PARAMETER_HPP
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
template <class TypeInfo = extensions::default_type_info>
class generic_parameter {
public:
  typedef void (*FunctionPtr)();
  virtual ~generic_parameter() {
    for (typename std::map<TypeInfo, basic_converter*>::iterator
         it = converters_.begin();
         it != converters_.end(); ++it) {
      delete it->second;
    }
  }
  virtual TypeInfo type() const = 0;
  template <class T>
  bool can_cast() const {
    TypeInfo i = type_info_handler<TypeInfo, T>::get_class_type();
    return (converters_.find(i) != converters_.end());
  }
  template <class S>
  S cast() const {
    S dest;
    TypeInfo i = type_info_handler<TypeInfo, S>::get_class_type();
    typename std::map<TypeInfo, basic_converter*>::const_iterator it =
      converters_.find(i);
    if (it != converters_.end()) {
      it->second->convert(value_, reinterpret_cast<void*>(&dest));
      return dest;
    }
    throw conversion_not_found_exception();
  }
  template <class S>
  void cast(S* dest) {
    *dest = cast<S>();
  }
protected:
  generic_parameter(void* value) : value_(value) {
  }
  class basic_converter {
  public:
    virtual void convert(void* src, void* dest) const = 0;
    virtual ~basic_converter() {}
  };
  std::map<TypeInfo, basic_converter*> converters_;
private:
  void* value_;
};

template <class T, class TypeInfo = extensions::default_type_info>
          class parameter : public generic_parameter<TypeInfo> {
public:
  template <class A, class B>
  friend class basic_parameter_map;

  virtual TypeInfo type() const {
    return reflections::type_info_handler<TypeInfo, T>::get_class_type();
  }
  explicit parameter(T value)
    : generic_parameter<TypeInfo>(reinterpret_cast<void*>(&value_)),
      value_(value) {
    // Add converter for current type.
    generic_parameter<TypeInfo>::converters_.insert
      (std::make_pair(reflections::type_info_handler<TypeInfo, T>
                        ::get_class_type(),
                      new default_converter<T>()));
  }
  template <class S>
  void converts_to_with_func(void (*convert_func)(T*, S*)) {
    generic_parameter<TypeInfo>::converters_.insert
      (std::make_pair(reflections::type_info_handler<TypeInfo, S>
                        ::get_class_type(),
                      new specialized_converter<S>(convert_func)));
  }
  template <class S>
  void converts_to() {
    generic_parameter<TypeInfo>::converters_.insert
      (std::make_pair(reflections::type_info_handler<TypeInfo, S>
                        ::get_class_type(),
                      new default_converter<S>()));
  }
private:
  
  template <class S>
  class default_converter :
    public generic_parameter<TypeInfo>::basic_converter {
  public:
    virtual void convert(void* val, void* dest) const {
      S* s = reinterpret_cast<S*>(dest);
      *s = static_cast<S>(*reinterpret_cast<T*>(val));
    }
  };
  template <class S>
  class specialized_converter :
    public generic_parameter<TypeInfo>::basic_converter {
  public:
    explicit specialized_converter(void (*convert_function)(T*, S*))
      : convert_function_(convert_function) {
    }
    virtual void convert(void* val, void* dest) const {
      S* s = reinterpret_cast<S*>(dest);
      (*convert_function_)(reinterpret_cast<T*>(val), s);
    }
  private:
    void (*convert_function_)(T*, S*);
  };
  T value_;
};
}
}

#endif // BOOST_REFLECTION_PARAMETER_HPP
