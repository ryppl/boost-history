// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_CONFIG_TUPLE_HPP_INCLUDED
#define BOOST_IDL_DETAIL_CONFIG_TUPLE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp> // BOOST_MSVC
#include <boost/detail/workaround.hpp>
#include <boost/mpl/limits/arity.hpp>
                  
#ifndef BOOST_IDL_MAX_FUNCTION_ARITY
#define BOOST_IDL_MAX_FUNCTION_ARITY 10
#endif

#define BOOST_IDL_MAX_TEMPLATE_ARITY BOOST_MPL_LIMIT_METAFUNCTION_ARITY
               
#define BOOST_IDL_GCC_VERSION \
    ( __GNUC__ * 10000 + \
      __GNUC_MINOR__ * 100 + \
      __GNUC_PATCHLEVEL__ ) 
    /**/

#ifndef BOOST_IDL_HAS_FULL_EBO
# if (BOOST_IDL_GCC_VERSION >= 30300)
#  define BOOST_IDL_HAS_FULL_EBO
# endif
#endif

#if defined(BOOST_IDL_OPTIMIZE_SIZE) && \
   !defined(BOOST_IDL_HAS_FULL_EBO)
# define BOOST_IDL_NO_MI
#endif

//#if BOOST_WORKAROUND(BOOST_MSVC, == 1310)
//# define BOOST_IDL_NO_FULL_GENERATOR_SUPPORT
//#endif

#endif // #ifndef BOOST_IDL_DETAIL_CONFIG_TUPLE_HPP_INCLUDED
