// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef CONFIG_DWA2004918_HPP
# define CONFIG_DWA2004918_HPP

#if !defined(BOOST_LANGBINDING_DYNAMIC_LIB)
#  define BOOST_PYTHON_DYNAMIC_LIB
#endif

#if defined(BOOST_LANGBINDING_DYNAMIC_LIB) && (defined(_WIN32) || defined(__CYGWIN__))
#  if defined(BOOST_LANGBINDING_SOURCE)
#     define BOOST_LANGBINDING_DECL __declspec(dllexport)
#     define BOOST_LANGBINDING_BUILD_DLL
#  else
#     define BOOST_LANGBINDING_DECL __declspec(dllimport)
#  endif
#endif

#ifndef BOOST_LANGBINDING_DECL
#  define BOOST_LANGBINDING_DECL
#endif

#endif // CONFIG_DWA2004918_HPP
