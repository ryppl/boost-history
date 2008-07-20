/*
 * Boost.Extension / factory map:
 *         map of factories (for the implementations)
 *
 * (C) Copyright Jeremy Pack 2008
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_EXTENSION_TYPE_MAP_HPP
#define BOOST_EXTENSION_TYPE_MAP_HPP

#include <map>
#include <boost/extension/impl/typeinfo.hpp>

namespace boost {
namespace extensions {

/** \brief A collection of types.
  * \tparam TypeInfo The type used for TypeInfo. By default,
  *         RTTI is used, but users can define their own TypeInfo.
  *         See impl/typeinfo.hpp.
  */
template <class TypeInfo>
class basic_type_map {
public:
  class type_map_convertible {
  public:
    ~type_map_convertible() {
      for (typename std::map<TypeInfo, generic_type_holder*>::iterator
           it =instances_.begin(); it != instances_.end(); ++it) {
        delete it->second;
      }
    }
    template <class Type>
    operator Type&() {
      TypeInfo t = type_info_handler<TypeInfo, Type>::get_class_type();
  
      typename std::map<TypeInfo, generic_type_holder*>::iterator
        it = instances_.find(t);
  
      type_holder<Type>* holder;
      if (it == instances_.end()) {
        holder = new type_holder<Type>;
        it = instances_.insert(std::make_pair(t, holder)).first;
      }
      else {
        holder = static_cast<type_holder<Type>* > (it->second);
      }
      return holder->val;
    }
  private:
    struct generic_type_holder {
      virtual ~generic_type_holder() {}
    };
    template <class T>
    struct type_holder : generic_type_holder {
      T val;
    };
    std::map<TypeInfo, generic_type_holder*> instances_;
  };

  type_map_convertible& get() {
    return convertible_;
  }

  template <class Type>
  Type& get() {
    return convertible_;
  }
private:
  type_map_convertible convertible_;

};
/** A typedef for convenience - provides the most common
  * type of basic_factory_map.
  */
typedef basic_type_map<default_type_info> type_map;

} // namespace extensions
} // namespace boost

#endif  // BOOST_EXTENSION_TYPE_MAP_HPP
