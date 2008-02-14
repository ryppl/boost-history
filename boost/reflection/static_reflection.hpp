/*
 * Boost.Reflection / static_reflection:
 *         reflections of static functions
 *
 * (C) Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_REFLECTION_STATIC_REFLECTION_HPP
#define BOOST_REFLECTION_STATIC_REFLECTION_HPP
#include <map>
#include <boost/extension/impl/typeinfo.hpp>
#include <boost/reflection/factory.hpp>

namespace boost { namespace reflections {
using extensions::type_info_handler;
#define BOOST_REFLECTION_FACTORY_MAP_GET_FUNCTION(Z, N, _) \
  template <class Info, class ReturnValue \
  BOOST_PP_COMMA_IF(N) \
  BOOST_PP_ENUM_PARAMS(N, class Param) > \
  std::map<Info, \
  ReturnValue (*)(BOOST_PP_ENUM_PARAMS(N, Param))> & get() { \
    typedef ReturnValue (*FuncType)(BOOST_PP_ENUM_PARAMS(N, Param)); \
    TypeInfo t = type_info_handler<TypeInfo, FuncType> \
    ::get_class_type(); \
    typename std::map<TypeInfo, generic_map_holder*>::iterator it = \
    maps_.find(t); \
    map_holder<std::map<Info, \
    FuncType> > * holder; \
    if (it == maps_.end()) { \
      holder = new map_holder<std::map<Info, FuncType > >; \
        it = maps_.insert(std::make_pair \
                          (t, holder)).first; \
    } else { \
      holder = \
      static_cast<map_holder<std::map<Info, FuncType> > *> \
      (it->second); \
    } \
    return *static_cast<std::map<Info, \
                        FuncType> * >(holder); \
  }
  
#define BOOST_REFLECTION_FACTORY_MAP_GET_FUNCTION_WITH_INFO(Z, N, _) \
  template <class Info, class ReturnValue \
  BOOST_PP_COMMA_IF(N) \
  BOOST_PP_ENUM_PARAMS(N, class Param) > \
  ReturnValue (*get(Info info))(BOOST_PP_ENUM_PARAMS(N, Param)) { \
    typedef ReturnValue (*FuncType)(BOOST_PP_ENUM_PARAMS(N, Param)); \
    std::map<Info, FuncType>& fm = get<Info, ReturnValue  BOOST_PP_COMMA_IF(N) \
                           BOOST_PP_ENUM_PARAMS(N, Param) >(); \
    typename std::map<Info, FuncType>::iterator it = fm.find(info); \
    if (it == fm.end()) { \
      return 0; \
    } \
    return it->second; \
  }
  
#define BOOST_REFLECTION_FACTORY_MAP_SET_FUNCTION(Z, N, _) \
  template <class Info, class ReturnValue \
  BOOST_PP_COMMA_IF(N) \
  BOOST_PP_ENUM_PARAMS(N, class Param) > \
  void set(Info info, ReturnValue (*func)(BOOST_PP_ENUM_PARAMS(N, Param))) { \
    typedef ReturnValue (*FuncType)(BOOST_PP_ENUM_PARAMS(N, Param)); \
    get<Info, ReturnValue  BOOST_PP_COMMA_IF(N) \
        BOOST_PP_ENUM_PARAMS(N, Param) >()[info] = func; \
  }

#define BOOST_REFLECTION_FACTORY_MAP_CONVERT_FUNCTION(Z, N, _) \
  template <class Info, class ReturnValue \
  BOOST_PP_COMMA_IF(N) \
  BOOST_PP_ENUM_PARAMS(N, class Param) > \
  operator std::map<Info, \
  ReturnValue (*)(BOOST_PP_ENUM_PARAMS(N, Param))>() { \
    return get<Info, ReturnValue BOOST_PP_COMMA_IF(N) \
    BOOST_PP_ENUM_PARAMS(N, Param)>(); \
  }
  
template <class TypeInfo>
class basic_static_reflection {
public:
  BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS), \
                  BOOST_REFLECTION_FACTORY_MAP_GET_FUNCTION, _)
  BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS), \
                  BOOST_REFLECTION_FACTORY_MAP_GET_FUNCTION_WITH_INFO, _)
  BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS), \
                  BOOST_REFLECTION_FACTORY_MAP_SET_FUNCTION, _)
  BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS), \
                  BOOST_REFLECTION_FACTORY_MAP_CONVERT_FUNCTION, _)
  ~basic_static_reflection() {
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
typedef basic_static_reflection<extensions::default_type_info> static_reflection;
}}

#undef BOOST_REFLECTION_STATIC_REFLECTION_GET_FUNCTION
#undef BOOST_REFLECTION_STATIC_REFLECTION_CONVERT_FUNCTION
#endif
