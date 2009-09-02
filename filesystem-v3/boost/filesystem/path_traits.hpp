//  filesystem path_traits.hpp  --------------------------------------------------------//

//  Copyright Beman Dawes 2009

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  Library home page: http://www.boost.org/libs/filesystem

/*
   FAQ

   Why are there no error_code & arguments?
   ----------------------------------------

   error_code & arguments add considerably to the surface area of the interface, yet
   appear to be of limited usefulness. They have not been requested by users; the need
   for filesystem error reporting via error_code seems limited to operational failures
   rather than path failures.

   error_code & arguments double the number of signatures, since for efficiency the
   use of a default throws() argument is not desirable.

   Errors in path conversions only occur if the source and target value types differ AND
   the target encoding can't represent a character present in the source. The only
   commonplace case is when directory iteration on Windows encounters a file name that
   can't be represented in a char encoding.

   Workarounds (such as pre-scanning for characters that can't be encoded) appear
   resonable.

   Why are there no const codecvt_type & arguments?
   ------------------------------------------------

   To hold down the size of the class path interface. Per function codecvt facets
   just aren't needed very often in practice.

   An RAII idiom can be used to ensure push/pop behavior as an alternative.

   Note that codecvt() is passed to the path_traits::convert functions, since that
   decouples the convert functions from class path.

   const codecvt_type & can be added later, but once added, they can never be removed
   since that would break user code.
*/

#ifndef BOOST_FILESYSTEM_PATH_TRAITS_HPP
#define BOOST_FILESYSTEM_PATH_TRAITS_HPP

#include <boost/filesystem/config.hpp>
#include <string>
#include <iterator>
#include <boost/assert.hpp>
#include <boost/system/error_code.hpp>
// #include <iostream>   //**** comment me out ****

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost { namespace filesystem {

  class directory_entry;
  
namespace path_traits {
 
  typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_type;

  //  Pathable empty

  template <class Container> inline
    bool empty( const Container & c )
      { return c.begin() == c.end(); }

  template <class T> inline
    bool empty( T * const & c_str )
  {
    BOOST_ASSERT( c_str );
    return !*c_str;
  }

  template <typename T, size_t N> inline
     bool empty( T (&array)[N] )
       { return N <= 1; }

  //  Pathable dispatch

  template <class Container, class U> inline
  void dispatch( const Container & c, U & to, const codecvt_type & cvt )
  {
//    std::cout << "dispatch() container\n";
    if ( c.size() )
      convert( &*c.begin(), &*c.begin() + c.size(), to, cvt );
  }

  template <class T, class U> inline
  void dispatch( T * const & c_str, U & to, const codecvt_type & cvt )
  {
//    std::cout << "dispatch() const T *\n";
    BOOST_ASSERT( c_str );
    convert( c_str, to, cvt );
  }
  
  template <typename T, size_t N, class U> inline
  void dispatch( T (&array)[N], U & to, const codecvt_type & cvt ) // T, N, U deduced
  {
//    std::cout << "dispatch() array, N=" << N << "\n"; 
    convert( array, array + N - 1, to, cvt );
  }

# ifdef BOOST_WINDOWS_API

  BOOST_FILESYSTEM_DECL
  void dispatch( const directory_entry & de, std::wstring & to, const codecvt_type & );

# else

  BOOST_FILESYSTEM_DECL
  void dispatch( const directory_entry & de, std::string & to, const codecvt_type & );

# endif

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

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif  // BOOST_FILESYSTEM_PATH_TRAITS_HPP
