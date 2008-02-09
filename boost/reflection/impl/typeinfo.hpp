/*
 * Boost.Reflection / typeinfo:
 *         implementations name management with RTTI
 *
 * (C) Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */


#ifndef BOOST_REFLECTION_TYPEINFO_HPP
#define BOOST_REFLECTION_TYPEINFO_HPP
namespace boost{namespace reflections{
template <class TypeInfo, class ClassType>
struct type_info_handler
{
  static TypeInfo get_class_type();
};
}}
#ifndef BOOST_REFLECTION_USER_TYPE_INFO_CUSTOM
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
 //  For DLLs
#  define BOOST_REFLECTION_EXTERNAL extern "C" __declspec(dllexport) 
#include <string>
#include <typeinfo>
namespace boost{namespace reflections{
  typedef std::string default_type_info;
  template <class ClassType>
    struct type_info_handler<default_type_info, ClassType>
  {
    static default_type_info get_class_type(){return typeid(ClassType).name();}
  };
}}

#else
#ifdef __APPLE__
#include <typeinfo>
#include <string>
/*namespace boost{namespace reflections{
  typedef basic_factory_map<std::type_info> factory_map;
  template <>
    std::type_info basic_factory_map<std::string>::get_class_type()
  {
      return typeid(ClassType);
  }
}}*/
namespace boost{namespace reflections{
  typedef std::string default_type_info;
  template <class ClassType>
    struct type_info_handler<default_type_info, ClassType>
  {
    static default_type_info get_class_type(){return typeid(ClassType).name();}
  };
}}

#else
#include <string>
#include <typeinfo>
namespace boost{namespace reflections{
  typedef std::string default_type_info;
  template <class ClassType>
    struct type_info_handler<default_type_info, ClassType>
  {
    static default_type_info get_class_type(){return typeid(ClassType).name();}
  };
}}

#endif //Apple
#endif //Windows
#endif // BOOST_REFLECTION_USER_TYPE_INFO_CUSTOM
#ifndef BOOST_REFLECTION_EXTERNAL
#  define BOOST_REFLECTION_EXTERNAL extern "C"
#endif

#endif
