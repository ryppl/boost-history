//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_DETAIL_CONFIG_HPP)
#define BOOST_SQL_CLI_DETAIL_CONFIG_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_DECLSPEC // defined in config system
// we need to import/export our code only if the user has specifically
// asked for it by defining either BOOST_ALL_DYN_LINK if they want all boost
// libraries to be dynamically linked, or BOOST_SQL_CLI_DYN_LINK
// if they want just this one to be dynamically liked:
#   if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_SQL_CLI_DYN_LINK)
// export if this is our own source, otherwise import:
#       ifdef BOOST_SQL_CLI_SOURCE
#           define BOOST_SQL_CLI_DECL __declspec(dllexport)
#       else
#           define BOOST_SQL_CLI_DECL __declspec(dllimport)
#       endif  // BOOST_SQL_CLI_SOURCE
#   endif  // DYN_LINK
#endif  // BOOST_HAS_DECLSPEC
//
// if BOOST_SQL_CLI_DECL isn't defined yet define it now:
#ifndef BOOST_SQL_CLI_DECL
#   define BOOST_SQL_CLI_DECL
#endif

//
// Automatically link to the correct build variant where possible. 
// 
#if !defined(BOOST_ALL_NO_LIB) && !defined(BOOST_SQL_CLI_NO_LIB) && !defined(BOOST_SQL_CLI_SOURCE)
//
// Set the name of our library, this will get undef'ed by auto_link.hpp
// once it's done with it:
//
#define BOOST_LIB_NAME boost_sql_cli
//
// If we're importing code from a dll, then tell auto_link.hpp about it:
//
#if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_SQL_CLI_DYN_LINK)
#  define BOOST_DYN_LINK
#endif
//
// And include the header that does the work:
//
#include <boost/config/auto_link.hpp>
#endif  // auto-linking disabled
#endif // ! defined(BOOST_SQL_CLI_DETAIL_CONFIG_HPP)
