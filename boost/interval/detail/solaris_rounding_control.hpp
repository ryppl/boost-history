/* boost linux_rounding_control.hpp header file
 *
 * Copyright Jens Maurer 2000
 * Permission to use, copy, modify, sell, and distribute this software
 * is hereby granted without free provided that the above copyright notice
 * appears in all copies and that both that copyright notice and this
 * permission notice appear in supporting documentation,
 *
 * Jens Maurer makes no representations about the suitability of this
 * software for any purpose. It is provided "as is" without express or
 * implied warranty.
 *
 * See http://www.boost.org/ for most recent version including documentation.
 *
 * $Id$
 */

#ifndef BOOST_INTERVAL_DETAIL_SOLARIS_ROUNDING_CONTROL_HPP
#define BOOST_INTERVAL_DETAIL_SOLARIS_ROUNDING_CONTROL_HPP

#ifndef BOOST_INTERVAL_HPP
#error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

#if !defined(sun) && !defined(__sun__)
#error This header is only intended for Sun Solaris.
#endif

#ifdef __GNUC__

#ifdef __i386__
#include <boost/interval/detail/x86gcc_rounding_control.hpp>
#elif defined(sparc) || defined(__sparc__)
#include <boost/interval/detail/sparc_rounding_control.hpp>
#else
#error Unsupported CPU for Sun Solaris rounding control.
#endif

#elif defined(__KCC)

#include <boost/interval/detail/sparc_rounding_control.hpp>

#else
#error Unsupported compiler for Sun Solaris rounding control.
#endif

#endif /* BOOST_INTERVAL_DETAIL_SOLARIS_ROUNDING_CONTROL_HPP */
