// (C) Copyright 2003-2004: Reece H. Dunn; Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_DETAIL_CONFIG_HPP
#define BOOST_IOFM_DETAIL_CONFIG_HPP
#  if(( defined(_MSC_VER) && (_MSC_VER >= 1020 )) || defined(__WAVE__))
#     define BOOST_HAS_PRAGMA_ONCE
#  endif

#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/config.hpp>

   // custom configuration to identify supported boost libraries and
   // I/O formatting technologies available

   // Check for presence of hash containers and extensions

#  if defined(BOOST_HAS_HASH)
#     define BOOST_IOFM_HASH_CONTAINERS          // SGI hash containers
#     define BOOST_IOFM_HASH_MAP_T               typename KeyT, typename T, class Hash, class Cmp, class Allocator
#     define BOOST_IOFM_HASH_MAP_ARG             KeyT, T, Hash, Cmp, Allocator
#     define BOOST_IOFM_HASH_SET_T               typename KeyT, class Hash, class Cmp, class Allocator
#     define BOOST_IOFM_HASH_SET_ARG             KeyT, Hash, Cmp, Allocator
#  elif defined(BOOST_DINKUMWARE_STDLIB) && (BOOST_DINKUMWARE_STDLIB >= 306)
#     define BOOST_IOFM_HASH_CONTAINERS          // Dinkumware hash containers
#     define BOOST_IOFM_HASH_MAP_T               typename KeyT, typename T, class Cmp, class Allocator
#     define BOOST_IOFM_HASH_MAP_ARG             KeyT, T, Cmp, Allocator
#     define BOOST_IOFM_HASH_SET_T               typename KeyT, class Cmp, class Allocator
#     define BOOST_IOFM_HASH_SET_ARG             KeyT, Cmp, Allocator
#  endif

#  if defined(_CPPLIB_VER) && (_CPPLIB_VER >= 313)   // Dinkumware stdlib 313+
#     if defined(BOOST_MSVC) && (BOOST_MSVC >= 1310) // MS VC7.1+
#        define BOOST_IOFM_STDEXT                stdext
#     endif
#  endif

#  if !defined(BOOST_IOFM_STDEXT)
#     define BOOST_IOFM_STDEXT                   BOOST_STD_EXTENSION_NAMESPACE
#  endif

   // Generic configuration information

#  if defined(BOOST_NO_STD_LOCALE) // new I/O streams are broken or not implemented
#     define BOOST_IOFM_NO_BASIC_STREAM  // use old I/O streams
#  endif

   // Compiler/Library specific information

#  if defined(__BORLANDC__)
      // C++Compiler 5.5 - quaternion/octonion:
      //    does not like BOOST_QUATERNION_OPERATOR_GENERATOR(/)

#     define BOOST_IOFM_NO_LIB_QUATERNION
#     define BOOST_IOFM_NO_LIB_OCTONION
#  elif defined(__COMO__)
#     define BOOST_IOFM_NO_LIB_INTERVAL
#  elif defined(__GNUC__)
#     define BOOST_IOFM_NO_STD_WSTREAM

      // GNU 2.95.3-5 - quaternion/octonion: cannot find limits standard header file
      // GNU 3.3      - quaternion/octonion: many errors like
      //    boost/1.30.0/boost/math/quaternion.hpp:449:
      //       error: `R_component_2' undeclared (first use this function)

#     if( __GNUC__ == 2 )
#        define BOOST_IOFM_NO_LIB_QUATERNION
#        define BOOST_IOFM_NO_LIB_OCTONION
#        define BOOST_IOFM_NO_LIB_INTERVAL     // 'static_cast_expr' not supported by dump_expr
#        define BOOST_IOFM_NO_LIB_RATIONAL     // RHD: template instantiation depth exceeds maximum of 17:
#        define BOOST_IOFM_NO_OUTPUT_DEDUCTION // RHD: too many errors!
#     endif
#  elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC)
      // JDT: Intel 7.1 apparently has trouble with quaternion and octonian;
      // it also has trouble with the VC7.1 Dinkumware __declspec(deprecated)
      // on hashed containers. Apparently, the interval library is not configured
      // for intel.

#     define BOOST_IO_NO_DEPRECATED_MODIFIER
#     define BOOST_IOFM_NO_LIB_QUATERNION
#     define BOOST_IOFM_NO_LIB_OCTONION
#     define BOOST_IOFM_NO_LIB_INTERVAL
#  elif defined(__MWERKS__)
#  elif defined(_MSC_VER)
#     if( _MSC_VER < 1300 )
#        define BOOST_IOFM_NO_LIB_INTERVAL // JDT: trouble with interval/detail/bugs.hpp
#     endif
#  endif

// Include <istream>, <ostream> and <locale> if Dinkumware is in use but
// __declspec(deprecated) is not understood.
#  if defined(BOOST_IO_NO_DEPRECATED_MODIFIER) && defined(BOOST_DINKUMWARE_STDLIB)
#     define _DEPRECATED_TEMP _DEPRECATED
#     define _DEPRECATED
#     include <istream>
#     include <ostream>
#     include <locale>
#     define _DEPRECATED _DEPRECATED_TEMP
#  else
#     include <istream>
#     include <ostream>
#     include <locale>
#  endif

#endif
