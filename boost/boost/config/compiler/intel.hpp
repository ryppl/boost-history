//  (C) Copyright Boost.org 2001. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version.

//  Intel compiler setup:

#include "boost/config/compiler/common_edg.hpp"

#ifdef __ICL
#  define BOOST_COMPILER "Intel C++ version " BOOST_STRINGIZE(__ICL)
#  define BOOST_INTEL_CXX_VERSION __ICL
#else
#  define BOOST_COMPILER "Intel C++ version " BOOST_STRINGIZE(__ICC)
#  define BOOST_INTEL_CXX_VERSION __ICC
#endif

#if BOOST_INTEL_CXX_VERSION <= 500
#  define BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING
#endif

#ifdef _MSC_VER
#  ifndef _NATIVE_WCHAR_T_DEFINED
#     define BOOST_NO_INTRINSIC_WCHAR_T
#  endif
#  define BOOST_NO_SWPRINTF
#endif


//
// versions check:
// we don't support Intel prior to version 5.0:
#if BOOST_INTEL_CXX_VERSION < 500
#  error "Compiler not supported or configured - please reconfigure"
#endif
//
// last known and checked version is 500:
#if (BOOST_INTEL_CXX_VERSION > 500)
#  if defined(BOOST_ASSERT_CONFIG)
#     error "Unknown compiler version - please run the configure tests and report the results"
#  elif defined(_MSC_VER)
#     warning "Unknown compiler version - please run the configure tests and report the results"
#  endif
#endif

