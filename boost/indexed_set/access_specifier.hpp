/* Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_ACCESS_SPECIFIER_HPP
#define BOOST_INDEXED_SET_ACCESS_SPECIFIER_HPP

/* In those compilers that do not accept the member template friend syntax,
 * some protected and private sections might need to be specified as
 * public.
 */

#include <boost/config.hpp>

#if !defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)
#define BOOST_INDEXED_SET_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS protected
#define BOOST_INDEXED_SET_PRIVATE_IF_MEMBER_TEMPLATE_FRIENDS private
#else
#define BOOST_INDEXED_SET_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS public
#define BOOST_INDEXED_SET_PRIVATE_IF_MEMBER_TEMPLATE_FRIENDS public
#endif

#endif
