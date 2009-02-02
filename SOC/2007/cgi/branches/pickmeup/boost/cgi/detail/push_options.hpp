//              -- detail/push_options.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////

//#ifdef _MSC_VER

//#pragma warning (disable:)
// You may want to remove these.
#define _CRT_SECURE_NO_DEPRECATE 1
#define _SCL_SECURE_NO_WARNINGS 1
#define _CRT_SECURE_NO_WARNINGS 1
//#pragma warning (disable:4996)

//#endif

#if !defined(BOOST_CGI_INLINE)
#  if defined(BOOST_CGI_BUILD_LIB)
#    define BOOST_CGI_INLINE
#  else
#    define BOOST_CGI_INLINE inline
#  endif
#endif

/// Keep empty query string variables.
/** Empty query string parameters (eg.
 *   `empty` in /path/to/script?empty&foo=bar)
 * aren't guaranteed by the CGI spec to be kept, but you might want to use
 * them. You just have to define `BOOST_CGI_KEEP_EMPTY_VARS` (**FIXME**
 * currently on by default).
 */
#define BOOST_CGI_KEEP_EMPTY_VARS
