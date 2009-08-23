//  filesystem path.hpp  ---------------------------------------------------------------//

//  Copyright Beman Dawes 2002-2005, 2008
//  Copyright Vladimir Prus 2002

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  Library home page: http://www.boost.org/libs/filesystem

//  path::stem(), extension(), and replace_extension() are based on
//  basename(), extension(), and change_extension() from the original
//  filesystem/convenience.hpp header by Vladimir Prus.

//--------------------------------------------------------------------------------------//
/*
                              TO DO

   * Document breaking changes, provide workarounds where possible.
   * Windows, POSIX, conversions for char16_t, char32_t for supporting compilers.
   * Move semantics and other C++0x features.
   * Add Alternate Data Stream test cases. See http://en.wikipedia.org/wiki/NTFS Features.
   * test case: relational on paths differing only in trailing separator. rationale?
   * Behavior of root_path() has been changed. Change needs to be propagated to trunk.
     reference.html#Path-decomposition-table needs rows for //, //netname, //netname/foo
     Make sure current trunk has passing tests for those cases, all decompositions.
     See test_decompositions() in path_unit_test.cpp
   * Get the deprecated names working.
   * Document leading //: no longer treated specially.
   * Document behavior of path::replace_extension has change WRT argument w/o a dot.
   * reference.html: operator /= is underspecified as to when a "/" is appended, and
     whether a '/' or '\' is appended.
   * Provide the name check functions for more character types? Templatize?
   * Why do preferred() and generic() return paths rather than const strings/string refs?
     Either change or document rationale.
   * Use BOOST_DELETED, BOOST_DEFAULTED, where appropriate.
   * Add test for scoped_path_locale.
     
                         Design Questions


   * Should path_locale use thread local storage?
   * Will locale overloads be needed in practice?
   * Is it OK for single-element decomposition functions to return paths? Yes;
     keep the interface simple and usable in generic code at the expense of some
     notational convenience.
   * Are generic versions of string(), native_string() needed? IE:
        template< class T >
        T string( const error_code ec = throws() );
     TODO: Yes; all member functions need to be usable in generic code.
   * Assuming generic versions of string(), native_string(), are the w flavors needed?
     No. KISS. basic_string<char> is special because it is the predominent
     use case. w (and other) flavors can be added later.
   * Should UDT's be supported? Yes. Easy to do and pretty harmless.
   * Remove all support for environments that don't support wstring.
   * Should path iteration to a separator result in:
       -- the actual separator used
       -- the preferred separator
       -- the generic separator <-- makes it easier to write portable code
                                                                                        */
//--------------------------------------------------------------------------------------// 

#ifndef BOOST_FILESYSTEM_PATH_HPP
#define BOOST_FILESYSTEM_PATH_HPP

#include <boost/filesystem/config.hpp>
#include <boost/filesystem/path_traits.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/iterator/iterator_facade.hpp>
//#include <boost/utility/enable_if.hpp>
#include <boost/throw_exception.hpp>
#include <boost/shared_ptr.hpp>
//#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>
#include <string>
#include <cstring>
#include <iosfwd>    // needed by basic_path inserter and extractor
#include <stdexcept>
#include <cassert>
#include <locale>
#include <algorithm>

# ifdef BOOST_WINDOWS_API
# else   // BOOST_POSIX_API
# endif


#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost
{
namespace filesystem
{

  //------------------------------------------------------------------------------------//
  //                              class path_error                                      //
  //------------------------------------------------------------------------------------//

  //  filesystem_error is not used because errors are sometimes thrown during 
  //  path construction when there isn't a complete path to include.
  //  system_error is not used because most uses will be conversion errors, and
  //  thus it is useful to include the source string causing the error. Since
  //  processing source string errors is by its nature type dependent, the
  //  exception class is templated on the string type.
  
  //  path_error can be caught when further type distinctions aren't needed.

  class path_error : public std::runtime_error
  {
  public:
    path_error( const char * what_, boost::system::error_code ec_ )
      : std::runtime_error( what_ ), m_ec( ec_ ) {}

    boost::system::error_code  error_code() const throw() { return m_ec; }

  private:
    boost::system::error_code  m_ec;
  };

  template < class String >
  class basic_path_error : public path_error
  {
  public:
    basic_path_error( const char * what_, boost::system::error_code ec_,
      const String & source_ )
        : path_error( what_, ec_ ), m_source( source_ ) {}

    const String & native() const { return m_source; }

  private:
    String m_source;
  };

  //------------------------------------------------------------------------------------//
  //                                                                                    //
  //                                    class path                                      //
  //                                                                                    //
  //------------------------------------------------------------------------------------//

  class BOOST_FILESYSTEM_DECL path
  {
  public:

    //  value_type is the character type used by the operating system API to
    //  represent paths.

# ifdef BOOST_WINDOWS_API
    typedef wchar_t                        value_type;
    static const wchar_t preferred_separator = L'\\';
# else 
    typedef char                           value_type;
    static const char preferred_separator = '/';
# endif
    typedef std::basic_string<value_type>  string_type;  
    typedef string_type::size_type         size_type;
    typedef path_traits::codecvt_type      codecvt_type;

    //  ----- character encoding conversions -----

    //  Following the principle of least astonishment, path input arguments
    //  passed to or obtained from the operating system via objects of
    //  class path behave as if they were directly passed to or
    //  obtained from the O/S API, unless conversion is explicitly requested.
    //
    //  POSIX specfies that path strings are passed unchanged to and from the
    //  API. Note that this is different from the POSIX command line utilities,
    //  which convert according to a locale.
    //
    //  Thus for POSIX, char strings do not undergo conversion.  wchar_t strings
    //  are converted to/from char using the path locale or, if a conversion
    //  argument is given, using a conversion object modeled on
    //  std::wstring_convert.
    //
    //  The path locale, which is global to the thread, can be changed by the
    //  imbue() function. It is initialized to an implementation defined locale.
    //  
    //  For Windows, wchar_t strings do not undergo conversion. char strings
    //  are converted using the "ANSI" or "OEM" code pages, as determined by
    //  the AreFileApisANSI() function, or, if a conversion argument is given,
    //  using a conversion object modeled on std::wstring_convert.
    //
    //  See m_path comments for further important rationale.

    //  Design alternative; each function can have an additional overload that
    //  supplies a conversion locale. For example:
    //
    //      template< class ForwardIterator, class WStringConvert >
    //      path( ForwardIterator begin, ForwardIterator end,
    //            const std::locale & loc,
    //            system::error_code & ec = boost::throws() );
    // 
    //  This alternative was rejected as too complex for the limited benefits;
    //  it nearly doubles the size of the interface, and adds a lot of
    //  implementation and test code, yet would likely be rarely used. The same
    //  effect can be achieved via the much simpler imbue() mechanism.


    //  TODO: rules needed for operating systems that use / or .
    //  differently, or format directory paths differently from file paths. 
    //
    //  ************************************************************************
    //
    //  More work needed: How to handle an operating system that may have
    //  slash characters or dot characters in valid filenames, either because
    //  it doesn't follow the POSIX standard, or because it allows MBCS
    //  filename encodings that may contain slash or dot characters. For
    //  example, ISO/IEC 2022 (JIS) encoding which allows switching to
    //  JIS x0208-1983 encoding. A valid filename in this set of encodings is
    //  0x1B 0x24 0x42 [switch to X0208-1983] 0x24 0x2F [U+304F Kiragana letter KU]
    //                                             ^^^^
    //  Note that 0x2F is the ASCII slash character
    //
    //  ************************************************************************

    //  Supported source arguments: half-open iterator range, container, c-array,
    //  and single pointer to null terminated string.

    //  All source arguments except pointers to null terminated byte strings support
    //  multi-byte character string, which may have embedded nulls. Embedded null
    //  support is required for some Asian languages on Windows.

    //  -----  constructors  -----

    path(){}                                          

    path( const path & p ) : m_path(p.m_path) {}

    template <class ContiguousIterator>
    path( ContiguousIterator begin, ContiguousIterator end )
    { 
      if ( begin != end )
        path_traits::convert( &*begin, &*begin+std::distance(begin, end),
          m_path, codecvt() );
    }

    template <class PathSource>
    path( PathSource const & pathable )
    {
      path_traits::dispatch( pathable, m_path, codecvt() );
    }

    //  -----  assignments  -----

    path & operator=( const path & p )
    {
      m_path = p.m_path;
      return *this;
    }

    template <class ContiguousIterator>
    path & assign( ContiguousIterator begin, ContiguousIterator end )
    { 
      m_path.clear();
      if ( begin != end )
        path_traits::convert( &*begin, &*begin+std::distance(begin, end),
          m_path, codecvt() );
      return *this;
    }

    template <class PathSource>
    path & operator=( PathSource const & range )
    {
      m_path.clear();
      path_traits::dispatch( range, m_path, codecvt() );
      return *this;
    }

    //  -----  appends  -----

    //  if a separator is added, it is the preferred separator for the platform;
    //  slash for POSIX, backslash for Windows

    path & operator/=( const path & p )
    {
      m_append_separator_if_needed();
      m_path += p.m_path;
      return *this;
    }

    template <class ContiguousIterator>
    path & append( ContiguousIterator begin, ContiguousIterator end )
    { 
      m_append_separator_if_needed();
      if ( begin != end )
        path_traits::convert( &*begin, &*begin+std::distance(begin, end),
          m_path, codecvt() );
      return *this;
    }

    template <class PathSource>
    path & operator/=( PathSource const & range )
    {
      m_append_separator_if_needed();
      path_traits::dispatch( range, m_path, codecvt() );
      return *this;
    }

    //  -----  modifiers  -----

    void    clear()             { m_path.clear(); }
    void    swap( path & rhs )  { m_path.swap( rhs.m_path ); }
    path &  remove_filename();
    path &  replace_extension( const path & new_extension = path() );

#   ifdef BOOST_WINDOWS_API

    path & path::localize();  // change slash to backslash

#   else // BOOST_POSIX_API

    path & path::localize() { return *this; }  // POSIX m_path already localized

#   endif

//# ifndef BOOST_FILESYSTEM_NO_DEPRECATED
//    path &  normalize()         { return m_normalize(); }
//# endif

    //  -----  observers  -----
  
    //  For operating systems that format file paths differently than directory
    //  paths, return values from observers are formatted as file names unless there
    //  is a trailing separator, in which case returns are formatted as directory
    //  paths. POSIX and Windows make no such distinction.

    //  Implementations are permitted to return const values or const references.

    //  The string or path returned by an observer will be described as being formatted
    //  as "native" or "portable".
    //
    //  For POSIX, these are all the same format; slashes and backslashes are as input and
    //  are not modified.
    //
    //  For Windows,   native:    as input; slashes and backslashes are not modified;
    //                            this is the format of the internally stored string.
    //                 portable:  backslashes are converted to slashes

//    template< class T >  
//    T string( system::error_code & ec = boost::throws() ) const  // internal (i.e. original) format
//    {
//      return path_traits::convert<T>( m_path, ec );
//    }

    //  -----  native format observers  -----
    //
    //  access to the internal representation string is efficient and often convenient,
    //  but may result in less than fully portable code.

    const string_type &  native() const { return m_path; }
    const value_type *   c_str() const  { return m_path.c_str(); }

#   ifdef BOOST_WINDOWS_API

    const std::string     native_string() const;
    const std::wstring &  native_wstring() const { return m_path; }

#   else   // BOOST_POSIX_API

    const std::string &  native_string() const   { return m_path; }
    const std::wstring   native_wstring() const
    { 
      std::wstring tmp;
      if ( !m_path.empty() )
        path_traits::convert( &*m_path.begin(), &*m_path.begin()+m_path.size(),
          tmp, codecvt() );
      return tmp;
    }

#   endif

    //  -----  portable format observers  -----

#   ifdef BOOST_WINDOWS_API

    const std::string    string() const; 
    const std::wstring   wstring() const;

#   else // BOOST_POSIX_API

    const std::string &  string() const  { return m_path; }
    const std::wstring   wstring() const;

#   endif

    //  -----  decomposition  -----

    path  root_path() const; 
    path  root_name() const;         // returns 0 or 1 element path
                                     // even on POSIX, root_name() is non-empty() for network paths
    path  root_directory() const;    // returns 0 or 1 element path
    path  relative_path() const;
    path  parent_path() const;
    path  filename() const;          // returns 0 or 1 element path
    path  stem() const;              // returns 0 or 1 element path
    path  extension() const;         // returns 0 or 1 element path

    //  -----  query  -----

    bool empty() const               { return m_path.empty(); } // name consistent with std containers
    bool has_root_path() const       { return has_root_directory() || has_root_name(); }
    bool has_root_name() const       { return !root_name().empty(); }
    bool has_root_directory() const  { return !root_directory().empty(); }
    bool has_relative_path() const   { return !relative_path().empty(); }
    bool has_parent_path() const     { return !parent_path().empty(); }
    bool has_filename() const        { return !m_path.empty(); }
    bool is_complete() const
    {
#   ifdef BOOST_WINDOWS_PATH
      return has_root_name() && has_root_directory();
#   else
      return has_root_directory();
#   endif
    }

    //  -----  imbue  -----

    static std::locale imbue( const std::locale & loc );

    //  -----  codecvt  -----

    static const codecvt_type & codecvt()
    {
      return *wchar_t_codecvt_facet();
    }

    //  -----  iterators  -----

    class iterator;
    typedef iterator const_iterator;

    iterator begin() const;
    iterator end() const;

//--------------------------------------------------------------------------------------//
//                            class path private members                                //
//--------------------------------------------------------------------------------------//

  private:
/*
      m_path has the type, encoding, and format required by the native
      operating system. Thus for POSIX and Windows there is no conversion for
      passing m_path.c_str() to the O/S API or when obtaining a path from the
      O/S API. POSIX encoding is unspecified other than for dot and slash
      characters; POSIX just treats paths as a sequence of bytes. Windows
      encoding is UCS-2 or UTF-16 depending on the version.
*/
    string_type  m_path;  // Windows: as input; backslashes NOT converted to slashes,
                          // slashes NOT converted to backslashes

    void m_append_separator_if_needed();
    void m_portable();

    //path &  m_normalize();

    // Was qualified; como433beta8 reports:
    //    warning #427-D: qualified name is not allowed in member declaration 
    friend class iterator;
    friend bool operator<( const path & lhs, const path & rhs );

    static bool m_path_lex_compare( iterator first1, iterator last1,
      iterator first2, iterator last2 );

    // see path::iterator::increment/decrement comment below
    static void m_path_iterator_increment( path::iterator & it );
    static void m_path_iterator_decrement( path::iterator & it );

    static const codecvt_type *&  wchar_t_codecvt_facet();

  };  // class path

  //------------------------------------------------------------------------------------//
  //                             class path::iterator                                   //
  //------------------------------------------------------------------------------------//
 
  class path::iterator
    : public boost::iterator_facade<
      iterator,
      path const,
      boost::bidirectional_traversal_tag >
  {
  private:
    friend class boost::iterator_core_access;
    friend class boost::filesystem::path;
    friend void m_path_iterator_increment( path::iterator & it );
    friend void m_path_iterator_decrement( path::iterator & it );

    const path & dereference() const { return m_element; }

    bool equal( const iterator & rhs ) const
    {
      return m_path_ptr == rhs.m_path_ptr && m_pos == rhs.m_pos;
    }

    // iterator_facade derived classes don't seem to like implementations in
    // separate translation unit dll's, so forward to class path static members
    void increment() { m_path_iterator_increment( *this ); }
    void decrement() { m_path_iterator_decrement( *this ); }

    path                    m_element;   // current element
    const path *            m_path_ptr;  // path being iterated over
    string_type::size_type  m_pos;       // position of name in
                                         // m_path_ptr->m_path. The
                                         // end() iterator is indicated by 
                                         // m_pos == m_path_ptr->m_path.size()
  }; // path::iterator

  //------------------------------------------------------------------------------------//
  //                                                                                    //
  //                            class scoped_path_locale                                //
  //                                                                                    //
  //------------------------------------------------------------------------------------//
 
  class scoped_path_locale
  {
  public:
    scoped_path_locale( const std::locale & loc )
                      : m_saved_locale(loc)
    {
      path::imbue( loc );
    }

    ~scoped_path_locale()   // never throws()
    {
      try { path::imbue( m_saved_locale ); }
      catch ( ... ) {}
    };

  private:
    std::locale m_saved_locale;
  };
   
  //------------------------------------------------------------------------------------//
  //                                                                                    //
  //                              non-member functions                                  //
  //                                                                                    //
  //------------------------------------------------------------------------------------//

  //  relational operators act as if comparing native format strings

  inline bool operator<( const path & lhs, const path & rhs )
  {
    // because path iterators yield paths, std::lexicographical_compare 
    // infinately recurses, so use a path aware version
    return path::m_path_lex_compare(
      lhs.begin(), lhs.end(), rhs.begin(), rhs.end() );
  }

  inline bool operator<=( const path & lhs, const path & rhs ) { return !(rhs < lhs); }
  inline bool operator> ( const path & lhs, const path & rhs ) { return rhs < lhs; }
  inline bool operator>=( const path & lhs, const path & rhs ) { return !(lhs < rhs);  }

  // operator==() efficiency is a concern; a user reported the original version 2
  // !(lhs < rhs) && !(rhs < lhs) implementation caused a serious performance problem
  // for a map of 10,000 paths.

# ifdef BOOST_WINDOWS_API
  inline bool operator==( const path & lhs, const path::value_type * rhs )
  {
    const path::value_type * l(lhs.c_str());
    while ( (*l == *rhs || (*l == L'\\' && *rhs == L'/') || (*l == L'/' && *rhs == L'\\'))
      && *l ) { ++l; ++rhs; }
    return *l == *rhs || (*l == L'\\' && *rhs == L'/') || (*l == L'/' && *rhs == L'\\');  
  }
  inline bool operator==( const path & lhs, const path & rhs ) { return lhs == rhs.c_str(); }
  inline bool operator==( const path & lhs, const path::string_type & rhs ) { return lhs == rhs.c_str(); }
  inline bool operator==( const path::string_type & lhs, const path & rhs ) { return rhs == lhs; }
  inline bool operator==( const path::value_type * lhs, const path & rhs )  { return rhs == lhs; }
# else   // BOOST_POSIX_API
  inline bool operator==( const path & lhs, const path & rhs ) { return lhs.native() == rhs.native(); }
  inline bool operator==( const path & lhs, const path::string_type & rhs ) { return lhs.native() == rhs; }
  inline bool operator==( const path & lhs, const path::value_type * rhs )  { return lhs.native() == rhs; }
  inline bool operator==( const path::string_type & lhs, const path & rhs ) { return lhs == rhs.native(); }
  inline bool operator==( const path::value_type * lhs, const path & rhs )  { return lhs == rhs.native(); }
# endif


  inline bool operator!=( const path & lhs, const path & rhs ) { return !(lhs == rhs); }

  inline void swap( path & lhs, path & rhs )                   { lhs.swap( rhs ); }

  inline path operator/( const path & lhs, const path & rhs )  { return path( lhs ) /= rhs; }

  //  inserters and extractors

  inline std::ostream & operator<<( std::ostream & os, const path & p )
  {
    os << p.native_string();
    return os;
  }
  
  inline std::wostream & operator<<( std::wostream & os, const path & p )
  {
    os << p.native_wstring();
    return os;
  }
  
  inline std::istream & operator>>( std::istream & is, path & p )
  {
    std::string str;
    is >> str;
    p = str;
    return is;
  }
  
  inline std::wistream & operator>>( std::wistream & is, path & p )
  {
    std::wstring str;
    is >> str;
    p = str;
    return is;
  }

  //  name_checks

  BOOST_FILESYSTEM_DECL bool portable_posix_name( const std::string & name );
  BOOST_FILESYSTEM_DECL bool windows_name( const std::string & name );
  BOOST_FILESYSTEM_DECL bool portable_name( const std::string & name );
  BOOST_FILESYSTEM_DECL bool portable_directory_name( const std::string & name );
  BOOST_FILESYSTEM_DECL bool portable_file_name( const std::string & name );
  BOOST_FILESYSTEM_DECL bool native( const std::string & name );

}  // namespace filesystem
}  // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif  // BOOST_FILESYSTEM_PATH_HPP
