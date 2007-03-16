/* Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_TRY_CATCH_HPP
#define BOOST_INDEXED_SET_TRY_CATCH_HPP

/* After Robert Ramey's suggestion for coping with compilers
 * not supporting exceptions.
 */

#include <boost/config.hpp>

#if !defined(BOOST_NO_EXCEPTIONS)
#define BOOST_INDEXED_SET_TRY try
#define BOOST_INDEXED_SET_CATCH(x) catch(x)
#define BOOST_INDEXED_SET_RETHROW throw
#else
#define BOOST_INDEXED_SET_TRY 
#define BOOST_INDEXED_SET_CATCH(x) if(0)
#define BOOST_INDEXED_SET_RETHROW
#endif

#endif
