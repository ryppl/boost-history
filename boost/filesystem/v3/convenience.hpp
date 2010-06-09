//  boost/filesystem/convenience.hpp  ----------------------------------------//

//  Copyright Beman Dawes, 2002-2005
//  Copyright Vladimir Prus, 2002
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/filesystem

//----------------------------------------------------------------------------// 

#ifndef BOOST_FILESYSTEM3_CONVENIENCE_HPP
#define BOOST_FILESYSTEM3_CONVENIENCE_HPP

#include <boost/filesystem/v3/operations.hpp>
#include <boost/system/error_code.hpp>

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost
{
  namespace filesystem3
  {

# ifndef BOOST_FILESYSTEM_NO_DEPRECATED

    std::string extension(const path & p)
    {
      return p.extension().string();
    }

    std::string basename(const path & p)
    {
      return p.stem().string();
    }

    path change_extension( const path & p, const path & new_extension )
    { 
      path new_p( p );
      new_p.replace_extension( new_extension );
      return new_p;
    }

# endif


  } // namespace filesystem3
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas
#endif // BOOST_FILESYSTEM3_CONVENIENCE_HPP
