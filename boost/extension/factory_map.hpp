/*
 * Boost.Extension / factory map:
 *         map of factories (for the implementations)
 *
 * (C) Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_EXTENSION_FACTORY_MAP_HPP
#define BOOST_EXTENSION_FACTORY_MAP_HPP
#include <map>
#include <boost/extension/factory.hpp>
#include <boost/extension/impl/typeinfo.hpp>

namespace boost{namespace extensions{

#define BOOST_EXTENSION_FACTORY_MAP_GET_FUNCTION(Z, N, _) \
template <class Interface, class Info \
          BOOST_PP_COMMA_IF(N) \
          BOOST_PP_ENUM_PARAMS(N, class Param) > \
std::map<Info, \
  factory<Interface  BOOST_PP_COMMA_IF(N) \
  BOOST_PP_ENUM_PARAMS(N, Param)> > & get() { \
  TypeInfo t = type_info_handler<TypeInfo, \
    Interface *(*)(BOOST_PP_ENUM_PARAMS(N, Param))> \
  ::get_class_type(); \
  typename std::map<TypeInfo, generic_map_holder*>::iterator it = \
    maps_.find(t); \
  map_holder<std::map<Info, \
    factory<Interface BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, Param)> > > \
      * holder; \
  if (it == maps_.end()) { \
    holder = new map_holder<std::map<Info, \
      factory<Interface BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, Param)> > >; \
    it = maps_.insert(std::make_pair \
                      (t, holder)).first; \
  } else { \
    holder = \
    static_cast<map_holder<std::map<Info, \
      factory<Interface  BOOST_PP_COMMA_IF(N) \
              BOOST_PP_ENUM_PARAMS(N, Param)> > > *> \
    (it->second); \
  } \
  return *static_cast<std::map<Info, \
    factory<Interface BOOST_PP_COMMA_IF(N) \
            BOOST_PP_ENUM_PARAMS(N, Param)> > * >(holder); \
}

#define BOOST_EXTENSION_FACTORY_MAP_CONVERT_FUNCTION(Z, N, _) \
template <class Interface, class Info \
BOOST_PP_COMMA_IF(N) \
BOOST_PP_ENUM_PARAMS(N, class Param) > \
operator std::map<Info, \
factory<Interface  BOOST_PP_COMMA_IF(N) \
BOOST_PP_ENUM_PARAMS(N, Param)> >() { \
  return get<Interface, Info BOOST_PP_COMMA_IF(N) \
             BOOST_PP_ENUM_PARAMS(N, Param)>(); \
}

template <class TypeInfo>
class basic_factory_map {
public:
BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS), \
                BOOST_EXTENSION_FACTORY_MAP_GET_FUNCTION, _)
BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS), \
                BOOST_EXTENSION_FACTORY_MAP_CONVERT_FUNCTION, _)
  ~basic_factory_map() {
    for (typename std::map<TypeInfo, generic_map_holder*>
         ::iterator it =maps_.begin();
         it != maps_.end(); ++it) {
      delete it->second;
    }
  }
private:
  class generic_map_holder {
  public:
    virtual ~generic_map_holder() {}
  };
  template <class T>
  class map_holder : public generic_map_holder, public T{
  };
  std::map<TypeInfo, generic_map_holder*> maps_;
};
typedef basic_factory_map<default_type_info> factory_map;
}}

#undef BOOST_EXTENSION_FACTORY_MAP_GET_FUNCTION
#undef BOOST_EXTENSION_FACTORY_MAP_CONVERT_FUNCTION
#endif