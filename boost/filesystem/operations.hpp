//  boost/filesystem/operations.hpp  ---------------------------------------------------//

//  Copyright Beman Dawes 2010

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  Library home page: http://www.boost.org/libs/filesystem

//--------------------------------------------------------------------------------------// 

#ifndef BOOST_FILESYSTEM_OPERATIONSX_HPP
#define BOOST_FILESYSTEM_OPERATIONSX_HPP

# if defined(BOOST_FILESYSTEM_VERSION) \
  && BOOST_FILESYSTEM_VERSION != 2  && BOOST_FILESYSTEM_VERSION != 3
#   error BOOST_FILESYSTEM_VERSION defined, but not as 2 or 3
# endif

# if !defined(BOOST_FILESYSTEM_VERSION)
#   define BOOST_FILESYSTEM_VERSION 2
# endif

#if BOOST_FILESYSTEM_VERSION == 2
#  include <boost/filesystem/v2/operations.hpp>

# else
#  include <boost/filesystem/v3/operations.hpp>

# endif

#endif  // BOOST_FILESYSTEM_OPERATIONSX_HPP 
