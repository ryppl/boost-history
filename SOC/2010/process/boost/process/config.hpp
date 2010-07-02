//
// Boost.Process
// ~~~~~~~~~~~~~
//
// Copyright (c) 2006, 2007 Julio M. Merino Vidal
// Copyright (c) 2008 Ilya Sokolov
// Copyright (c) 2008, 2009 Boris Schaeling
// Copyright (c) 2010 Felipe Tanus, Boris Schaeling
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

/**
 * \file boost/process/config.hpp
 *
 * Defines macros that are used by the library's code to determine the
 * operating system it is running under and the features it supports.
 */

#ifndef BOOST_PROCESS_CONFIG_HPP
#define BOOST_PROCESS_CONFIG_HPP

#include <boost/config.hpp>
#include <boost/system/config.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/system/system_error.hpp>
#include <boost/throw_exception.hpp>

#if defined(BOOST_WINDOWS_API)
#   include <windows.h> 
#endif 

#if defined(BOOST_POSIX_API) || defined(BOOST_PROCESS_DOXYGEN)
#   if !defined(BOOST_PROCESS_POSIX_PATH_MAX)
/**
 * The macro BOOST_PROCESS_POSIX_PATH_MAX is set to a positive integer
 * value which specifies the system's maximal supported path length.
 * By default it is set to 259. You should set the macro to PATH_MAX
 * which should be defined in limits.h provided by your operating system
 * if you experience problems when instantiating a context. The
 * constructor of context tries to find out the maximal supported path 
 * length but uses BOOST_PROCESS_POSIX_PATH_MAX if it fails.
 */
#       define BOOST_PROCESS_POSIX_PATH_MAX 259
#   endif
#endif

#define BOOST_PROCESS_SOURCE_LOCATION \
    "in file '" __FILE__ "', line " BOOST_PP_STRINGIZE(__LINE__) ": "

#if defined(BOOST_POSIX_API)
#   define BOOST_PROCESS_LAST_ERROR errno
#elif defined(BOOST_WINDOWS_API)
#   define BOOST_PROCESS_LAST_ERROR ::GetLastError()
#endif

#define BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR(what) \
    boost::throw_exception(boost::system::system_error( \
        boost::system::error_code(BOOST_PROCESS_LAST_ERROR, \
                                  boost::system::get_system_category()), \
        BOOST_PROCESS_SOURCE_LOCATION what))

#endif