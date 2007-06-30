// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef CONFIG_DWA2004918_HPP
# define CONFIG_DWA2004918_HPP

#include <boost/config.hpp>

//#if !defined(BOOST_LANGBINDING_DYNAMIC_LIB) 
//#  define BOOST_PYTHON_DYNAMIC_LIB
//#endif

// compatibility with the rest of Boost's auto-linking code:
#if defined(BOOST_ALL_DYN_LINK)
# undef BOOST_LANGBINDING_STATIC_LIB
# define BOOST_LANGBINDING_DYNAMIC_LIB
#endif

#if defined(BOOST_LANGBINDING_DYNAMIC_LIB) && defined(BOOST_HAS_DECLSPEC)
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

//
// Automatically link to the correct build variant where possible. 
// 
//#if !defined(BOOST_ALL_NO_LIB) && !defined(BOOST_LANGBINDING_NO_LIB) && !defined(BOOST_WHATEVER_SOURCE)
//
// Tell the autolink to link dynamically, this will get undef'ed by auto_link.hpp
// once it's done with it: 
//
//#if defined(BOOST_LANGBINDING_DYNAMIC_LIB)
//#   define BOOST_DYN_LINK
//#endif
//
// Set the name of our library, this will get undef'ed by auto_link.hpp
// once it's done with it:
//
//#if defined(BOOST_LANGBINDING_LIB_NAME)
//#   define BOOST_LIB_NAME BOOST_LANGBINDING_LIB_NAME
//#else
//#   define BOOST_LIB_NAME boost_langbinding
//#endif
//
// And include the header that does the work:
//
//#include <boost/config/auto_link.hpp>
//#endif  // auto-linking disabled

#endif // CONFIG_DWA2004918_HPP
