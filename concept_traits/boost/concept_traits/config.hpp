
//  (C) Copyright Terje Slettebø and Tobias Schwinger 2004
//  Use, modification and distribution are subject to the Boost Software 
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#ifndef BOOST_CT_CONFIG_HPP_INCLUDED
#define BOOST_CT_CONFIG_HPP_INCLUDED

#include <cstddef>
#include "boost/config.hpp"

#define BOOST_CT_COMPILER_WITH_AT_LEAST_VERSION(compiler,ver) \
  (compiler >= ver)

#define BOOST_CT_COMPILER_BELOW_VERSION(compiler,ver) \
  ((compiler != 0) && (compiler < ver))

#define BOOST_CT_COMPILER_VERSIONS(compiler,min_ver,max_ver) \
  ((compiler != 0) && ((compiler >= min_ver) && (compiler <= max_ver)))

#if defined(BOOST_MSVC) \
|| (defined(__BORLANDC__) && !defined(BOOST_DISABLE_WIN32))
#
#   define BOOST_CT_ELLIPSIS_DECL __cdecl
#else
#   define BOOST_CT_ELLIPSIS_DECL /**/
#endif

#if defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
 || defined(BOOST_NO_ARRAY_TYPE_SPECIALIZATIONS)
 || BOOST_CT_COMPILER_BELOW_VERSION(__BORLANDC__,0x600)
 || defined(__IBMCPP__)
 || defined(__DMC__)
#
#   define BOOST_CT_NO_UNBOUNDED_ARRAYS
#endif

#endif // ndef BOOST_CT_CONFIG_HPP_INCLUDED

