//  filesystem path_traits.hpp  --------------------------------------------------------//

//  Copyright Beman Dawes 2009

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  Library home page: http://www.boost.org/libs/filesystem

#ifndef BOOST_FILESYSTEM_PATH_TRAITS_HPP
#define BOOST_FILESYSTEM_PATH_TRAITS_HPP

#include <boost/filesystem/config.hpp>
#include <string>
#include <iterator>
#include <boost/assert.hpp>
#include <boost/system/error_code.hpp>

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost { namespace filesystem { namespace path_traits {
  
  typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_type;

  // value types differ  ---------------------------------------------------------------//
  //
  //   A from_end argument of 0 is less efficient than a known end, so use only if needed
  
  BOOST_FILESYSTEM_DECL
  void convert( const char * from,
                const char * from_end,    // 0 for null terminated MBCS
                std::wstring & to,
                const codecvt_type & cvt );

  BOOST_FILESYSTEM_DECL
  void convert( const wchar_t * from,
                const wchar_t * from_end,  // 0 for null terminated MBCS
                std::string & to,
                const codecvt_type & cvt );

  inline 
  void convert( const char * from,
                std::wstring & to,
                const codecvt_type & cvt )
  {
    BOOST_ASSERT( from );
    convert( from, 0, to, cvt );
  }

  inline 
  void convert( const wchar_t * from,
                std::string & to,
                const codecvt_type & cvt )
  {
    BOOST_ASSERT( from );
    convert( from, 0, to, cvt );
  }

  // value types same  -----------------------------------------------------------------//

  // char

  inline 
  void convert( const char * from, const char * from_end, std::string & to,
    const codecvt_type & )
  {
    BOOST_ASSERT( from );
    BOOST_ASSERT( from_end );
    to.append( from, from_end );
  }

  inline 
  void convert( const char * from,
                std::string & to,
                const codecvt_type & )
  {
    BOOST_ASSERT( from );
    to += from;
  }

  // wchar_t

  inline 
  void convert( const wchar_t * from, const wchar_t * from_end, std::wstring & to,
    const codecvt_type & )
  {
    BOOST_ASSERT( from );
    BOOST_ASSERT( from_end );
    to.append( from, from_end );
  }

  inline 
  void convert( const wchar_t * from,
                std::wstring & to,
                const codecvt_type & )
  {
    BOOST_ASSERT( from );
    to += from;
  }

}}} // namespace boost::filesystem::path_traits

#endif  // BOOST_FILESYSTEM_PATH_TRAITS_HPP
