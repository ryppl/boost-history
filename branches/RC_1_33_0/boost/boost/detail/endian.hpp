/*
 * Copyright (c) 1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/*
 * Copyright notice reproduced from <boost/detail/limits.hpp>, from
 * which this code was originally taken.
 *
 * Modified by Caleb Epstein to use <endian.h> with GNU libc and to
 * defined the BOOST_ENDIAN macro.
 */

#ifndef BOOST_DETAIL_ENDIAN_HPP
#define BOOST_DETAIL_ENDIAN_HPP

// GNU libc offers the helpful header <endian.h> which defines
// __BYTE_ORDER

#if defined (__GLIBC__)
# include <endian.h>
# if (__BYTE_ORDER == __LITTLE_ENDIAN)
#  define BOOST_LITTLE_ENDIAN
# elif (__BYTE_ORDER == __BIG_ENDIAN)
#  define BOOST_BIG_ENDIAN
# elif (__BYTE_ORDER == __PDP_ENDIAN)
#  define BOOST_PDP_ENDIAN
# else
#  error Unknown machine endianness detected.
# endif
# define BOOST_BYTE_ORDER __BYTE_ORDER
#elif defined(__sparc) || defined(__sparc__) \
   || defined(_POWER) || defined(__powerpc__) \
   || defined(__ppc__) || defined(__hppa) \
   || defined(_MIPSEB) || defined(_POWER) \
   || defined(__s390__)
# define BOOST_BIG_ENDIAN
# define BOOST_BYTE_ORDER 4321
#elif defined(__i386__) || defined(__alpha__) \
   || defined(__ia64) || defined(__ia64__) \
   || defined(_M_IX86) || defined(_M_IA64) \
   || defined(_M_ALPHA)
# define BOOST_LITTLE_ENDIAN
# define BOOST_BYTE_ORDER 1234
#else
# error The file boost/detail/endian.hpp needs to be set up for your CPU type.
#endif


#endif
