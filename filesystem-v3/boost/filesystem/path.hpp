//  filesystem path.hpp  ---------------------------------------------------------------//

//  Copyright Beman Dawes 2002-2005, 2008
//  Copyright Vladimir Prus 2002

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/filesystem

//  basic_path's stem(), extension(), and replace_extension() are based on
//  basename(), extension(), and change_extension() from the original
//  filesystem/convenience.hpp header by Vladimir Prus.

//--------------------------------------------------------------------------------------//
/*
                              TO DO

   * Windows, POSIX, conversions for char16_t, char32_t for supporting compilers.
   * Windows, POSIX, conversions for user-defined types.
   * Add Alternate Data Stream test cases. See http://en.wikipedia.org/wiki/NTFS Features.
   * test case: relational on paths differing only in trailing separator. rationale?
   * Behavior of root_path() has been changed. Change needs to be propagated to trunk.
     reference.html#Path-decomposition-table needs rows for //, //netname, //netname/foo
     Make sure current trunk has passing tests for those cases, all decompositions.
     See test_decompositions() in path_unit_test.cpp
   * Get the deprecated names working.
   * path_test ~line 1200 replace_extension() tests commented out
   * Document leading //: no longer treated specially.
   * reference.html: operator /= is underspecified as to when a "/" is appended, and
     whether a '/' or '\' is appended.
   * path_unit_test needs to probe error handling, verify exceptions are thrown when
     requested.
   * Provide the name check functions for more character types. Templatize?
     
                         Design Questions

   * Should path_locale use thread local storage?
   * Will locale overloads be needed in practice?
   * Is it OK for single-element decomposition functions to return paths? Yes;
     keep the interface simple and usable in generic code at the expense of some
     notational convenience.
   * Are generic versions of string(), native_string() needed? IE:
        template< class T >
        T string( const error_code ec = throws );
     TODO: Yes; all member functions need to be usable in generic code.
     Can string() and native_string() make use of detail::convert()?
   * Assuming generic versions of string(), native_string(), are the w flavors needed?
     No. KISS. basic_string<char> is special because it is the predominent
     use case. w (and other) flavors can be added later.
   * Should UDT's be supported? Yes. Easy to do and pretty harmless.
   * What to do about Cygwin and other environments that don't support wchar_t?
   * Should path iteration to a separator result in:
       -- the actual separator used
       -- the preferred separator
       -- the generic separator <-- makes it easier to write portable code
   * Should the preferred native separator be available?
                                                                                        */
//--------------------------------------------------------------------------------------// 

#ifndef BOOST_FILESYSTEM_PATH_HPP
#define BOOST_FILESYSTEM_PATH_HPP

#include <boost/filesystem/config.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/throw_exception.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits/is_same.hpp>
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

//--------------------------------------------------------------------------------------//

namespace boost
{
namespace filesystem
{

  //  exception classes  -----------------------------------------------------//

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

    const String & source() const { return m_source; }

  private:
    String m_source;
  };

  //------------------------------------------------------------------------------------//
  //                                                                                    //
  //                           implementation details                                   //
  //                                                                                    //
  //------------------------------------------------------------------------------------//

  namespace detail
  {

    BOOST_FILESYSTEM_DECL extern const std::locale * path_locale;

#   ifdef BOOST_WINDOWS_API

    BOOST_FILESYSTEM_DECL
    void append( const char * begin,
                 const char * end,      // 0 for null terminated MBCS
                 std::wstring & target, system::error_code & ec );

    //  ----- convert -----

    BOOST_FILESYSTEM_DECL
    std::string convert_to_string( const std::wstring & src, system::error_code & ec ); 

# else   // BOOST_POSIX_API
 
    typedef std::string              string_type;
    typedef string_type::value_type  value_type;
    typedef string_type::size_type   size_type;

    //  See comment at equivalent location in Windows API detail above

    //  -----  char[] to string  -----

    inline void append( const char * begin,
                         const char * end,      // 0 for null terminated MBCS
                         std::string & target,
                         system::error_code & ec )
    {
      ec.clear(); 
      target.assign( begin, end ); // but what if this throws bad_alloc?
    }

    inline void append( const char * begin, std::string & target,
      system::error_code & ec )
    {
      ec.clear(); 
      target += begin; // but what if throws bad_alloc?
    }

#   ifndef BOOST_FILESYSTEM_NARROW_ONLY

    //  -----  wchar_t[] to string  -----  

    inline void append( const wchar_t * begin, const wchar_t * end,
      std::string & target, system::error_code & ec );

    inline void append( const wchar_t * begin, std::string & target,
      system::error_code & ec )
    { 
      append( begin, 0, target, ec );
    }

    //  ----- convert ----

    BOOST_FILESYSTEM_DECL
    std::wstring convert_to_wstring( const std::string & src, system::error_code & ec ); 

#   endif

# endif  // BOOST_POSIX_API

}  // namespace detail

  //------------------------------------------------------------------------------------//
  //                                                                                    //
  //                                path_traits                                         //
  //                                                                                    //
  //   Specializations are provided for char, wchar_t, char16_t, and char32_t value     //
  //   types and their related string and iterator types.                               //
  //                                                                                    //
  //   Users are permitted to add specializations for additional types.                 //
  //                                                                                    //
  //------------------------------------------------------------------------------------//

namespace path_traits
{

  // path representation type
#ifdef BOOST_WINDOWS_API
  typedef std::wstring string_type;  
#else 
  typedef std::string string_type;
#endif

  template< class I > struct is_iterator { static const bool value = false; };
  template< class C > struct is_container { static const bool value = false; };

  template< class charT >   // specialization optional
  inline void append( const charT * begin,   // requires: null termination
    string_type & target, system::error_code & ec )
  {
    path_traits::append<charT>( begin, 0, target, ec );
  }

  template< class charT >   // specialization required
  void append( const charT * begin, const charT * end,
               string_type & target, system::error_code & ec );

  template< class String >   // specialization required
  String convert( const string_type & source, system::error_code & ec );

  //------------------------------------------------------------------------------------//
  //                              specializations                                       //
  //------------------------------------------------------------------------------------//

  template<> struct is_iterator<const char *> { static const bool value = true; };
  template<> struct is_iterator<char *> { static const bool value = true; };
  template<> struct is_iterator<std::string::iterator> { static const bool value = true; };
  template<> struct is_iterator<std::string::const_iterator> { static const bool value = true; };
  template<> struct is_container<std::string> { static const bool value = true; };

  template<> struct is_iterator<const wchar_t *> { static const bool value = true; };
  template<> struct is_iterator<wchar_t *> { static const bool value = true; };
  template<> struct is_iterator<std::wstring::iterator> { static const bool value = true; };
  template<> struct is_iterator<std::wstring::const_iterator> { static const bool value = true; };
  template<> struct is_container<std::wstring> { static const bool value = true; };

  template<>
  inline void append<string_type::value_type>( const string_type::value_type * begin,
    const string_type::value_type * end, string_type & target, system::error_code & ec )
  {
    ec.clear(); 
    target.assign( begin, end ); // but what if throws bad_alloc?
  }

  template<>
  inline void append<string_type::value_type>( const string_type::value_type * begin,
    string_type & target, system::error_code & ec )
  {
    ec.clear(); 
    target += begin; // but what if throws bad_alloc?
  }

  template<>
  inline string_type convert<string_type>( const string_type & s, system::error_code & ec )
  { 
    return s;
  }

# ifdef BOOST_WINDOWS_API

  template<>
  inline void append<char>( const char * begin, const char * end,
    std::wstring & target, system::error_code & ec )
  {
    detail::append( begin, end, target, ec );
  }

  template<>
  inline void append<char>( const char * begin, std::wstring & target,
     system::error_code & ec )
  { 
    detail::append( begin, 0, target, ec );
  }

  template<>
  inline std::string convert<std::string>( const std::wstring & s,
    system::error_code & ec )
  {
    return detail::convert_to_string( s, ec );
  }

# endif

#   ifdef BOOST_FILESYSTEM_CPP0X_CHAR_TYPES
      ...
#   endif

}  // namespace path_traits

  //------------------------------------------------------------------------------------//
  //                                                                                    //
  //                                    class path                                      //
  //                                                                                    //
  //------------------------------------------------------------------------------------//

  class BOOST_FILESYSTEM_DECL path
  {
  public:

    //  string_type is the std::basic_string type corresponding to the character
    //  type for paths used by the native operating system API.
    //
    //  Thus string_type is std::string for POSIX and std::wstring for Windows.
    //  value_type is char for POSIX and wchar_t for Windows.

    typedef path_traits::string_type  string_type;
    typedef string_type::value_type   value_type;
    typedef string_type::size_type    size_type;

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
    //            system::error_code & ec = system::throws );
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

    //  -----  constructors  -----

    path(){}                                          // #1

    path( const path & p ) : m_path(p.m_path) {}      // #2

    //  construct from null terminated sequence

    template< class InputIterator >
      path( InputIterator begin,
        system::error_code & ec = system::throws,
        typename boost::enable_if<path_traits::is_iterator<InputIterator> >::type* dummy=0 )  // #3
          { m_append( begin, m_path, ec ); }

    //  construct from (potentially) multi-byte character string, which may have
    //  embedded nulls. Embedded null support is required for some Asian languages on
    //  Windows.

    template< class ForwardIterator >
      path( ForwardIterator begin, ForwardIterator end,
        system::error_code & ec = system::throws )                                 // #4
          { m_append( begin, end, m_path, ec ); }

    //  construct from container of (potentially) multi-byte character string,
    //  which may have embedded nulls.  Embedded null support is required for
    //  some Asian languages on Windows.

    template< class Container >
      path( const Container & ctr,
        system::error_code & ec = system::throws,
        typename boost::enable_if<path_traits::is_container<Container> >::type* dummy=0 )  // #5
          { m_append( ctr.begin(), ctr.end(), m_path, ec ); }


    //  -----  assignments  -----

    path & operator=( const path & p )                                     // #1
    {
      m_path = p.m_path;
      return *this;
    }

    template< class InputIterator >
      typename boost::enable_if<path_traits::is_iterator<InputIterator>, path &>::type
        operator=( InputIterator begin )                                   // #2
    {
      m_path.clear();
      m_append( begin, m_path, system::throws );
      return *this;
    }

    template< class InputIterator >
      typename boost::enable_if<path_traits::is_iterator<InputIterator>, path &>::type
        assign( InputIterator begin,
          system::error_code & ec = system::throws )                       // #3
    {
      m_path.clear();
      m_append( begin, m_path, ec );
      return *this;
    }

    template< class FowardIterator >
      path & assign( FowardIterator begin, FowardIterator end,
        system::error_code & ec = system::throws )                         // #4
    { 
      m_path.clear();
      m_append( begin, end, m_path, ec );
      return *this;
    }
 
    template< class Container >
      typename boost::enable_if<path_traits::is_container<Container>, path &>::type 
        operator=( const Container & ctr )                                 // #5
    { 
      m_path.clear();
      m_append( ctr.begin(), ctr.end(), m_path, system::throws );
      return *this;
    }
 
    template< class Container >
      typename boost::enable_if<path_traits::is_container<Container>, path &>::type
        assign( const Container & ctr,
        system::error_code & ec = system::throws )                         // #6
    { 
      m_path.clear();
      m_append( ctr.begin(), ctr.end(), m_path, ec );
      return *this;
    }

    //  -----  appends  -----

    path & operator/=( const path & p )                                    // #1
    {
      append_separator_if_needed_();
      m_path += p.m_path;
      return *this;
    }

    template< class InputIterator >
      typename boost::enable_if<path_traits::is_iterator<InputIterator>, path &>::type
        operator/=( InputIterator begin )                                  // #2
    {
      append_separator_if_needed_();
      m_append( begin, m_path, system::throws );
      return *this;
    }

    template< class InputIterator >
      typename boost::enable_if<path_traits::is_iterator<InputIterator>, path &>::type
        append( InputIterator begin,
          system::error_code & ec = system::throws )                       // #3
    {
      append_separator_if_needed_();
      m_append( begin, m_path, ec );
      return *this;
    }

    template< class FowardIterator >
      path & append( FowardIterator begin, FowardIterator end,
        system::error_code & ec = system::throws )                         // #4
    { 
      append_separator_if_needed_();
      m_append( begin, end, m_path, ec );
      return *this;
    }
 
    template< class Container >
      typename boost::enable_if<path_traits::is_container<Container>, path &>::type
        operator/=( const Container & ctr )                                // #5
    { 
      append_separator_if_needed_();
      m_append( ctr.begin(), ctr.end(), m_path, system::throws );
      return *this;
    }
 
    template< class Container >
      typename boost::enable_if<path_traits::is_container<Container>, path &>::type
        append( const Container & ctr,
        system::error_code & ec = system::throws )                         // #6
    { 
      append_separator_if_needed_();
      m_append( ctr.begin(), ctr.end(), m_path, ec );
      return *this;
    }

    //  -----  modifiers  -----

    void clear() { m_path.clear(); }

    void swap( path & rhs )
    {
      m_path.swap( rhs.m_path );
#     ifdef BOOST_CYGWIN_PATH
        std::swap( m_cygwin_root, rhs.m_cygwin_root );
#     endif
    }

    path & remove_filename();

    //  -----  conversion operators  -----

    //  return formatted "as input"

#   ifdef BOOST_WINDOWS_API

    operator const std::string() const    { return detail::convert_to_string( m_path, system::throws ); }
    operator const std::wstring&() const  { return m_path; }

# ifdef BOOST_FILESYSTEM_CPP0X_CHAR_TYPES
    operator const std::u16string() const { return detail::convert_to_u16string( m_path, system::throws ); }
    operator const std::u32string() const { return detail::convert_to_u32string( m_path, system::throws ); }
# endif

#   else   // BOOST_POSIX_API

    operator const std::string&() const   { return m_path; }
#     ifndef BOOST_FILESYSTEM_NARROW_ONLY
    operator const std::wstring() const   { return detail::convert( m_path, system::throws ); }
#     endif

#   endif

    //  -----  observers  -----
  
    //  For operating systems that format file paths differently than directory
    //  paths, return values from observers are formatted as file names unless there
    //  is a trailing separator, in which case returns are formatted as directory
    //  paths. POSIX and Windows make no such distinction.

    //  Implementations are permitted to return const values or const references.

    template< class T >
    T string( system::error_code & ec = system::throws ) const
    {
      return path_traits::convert<T>( m_path, ec );
    }

#   ifdef BOOST_WINDOWS_API

    //  return value is formatted "as input"
    const std::string     string( system::error_code & ec = system::throws ) const { return detail::convert_to_string( m_path, ec ); }
    const std::wstring &  wstring() const                                          { return m_path; }
    const std::wstring &  wstring( system::error_code & ec ) const                 { ec.clear(); return m_path; }

#   ifdef BOOST_FILESYSTEM_CPP0X_CHAR_TYPES
     ...
#   endif

#   else   // BOOST_POSIX_API

    //  return value is formatted "as input"
    const std::string &  string() const                                            { return m_path; }
    const std::string &  string( system::error_code & ec ) const                   { ec.clear(); return m_path; }
#     ifndef BOOST_FILESYSTEM_NARROW_ONLY
    const std::wstring   wstring( system::error_code & ec = system::throws ) const { return detail::convert( m_path, ec ); }
#     endif    
#   endif

    
#   ifdef BOOST_WINDOWS_PATH

    //  return value is formatted as indicated by function name
    const path  native() const;
    const path  generic() const;

#   else // BOOST_POSIX_PATH

    //  return value is formatted as indicated by function name
    const path  native() const   { return m_path; }
    const path  generic() const  { return m_path; }

#   endif

    const value_type *  c_str() const { return m_path.c_str(); }  // native format

    //  -----  decomposition  -----

    path  root_path() const; 
    path  root_name() const;        // returns 0 or 1 element path
                                    // even on POSIX, root_name() is non-empty() for network paths
    path  root_directory() const;   // returns 0 or 1 element path
    path  relative_path() const;
    path  parent_path() const;
    path  filename() const;         // returns 0 or 1 element path
    path  stem() const;             // returns 0 or 1 element path
    path  extension() const;        // returns 0 or 1 element path

    //  -----  query  -----

    bool empty() const               { return m_path.empty(); } // name consistent with std containers
    bool has_root_path() const       { return has_root_directory() || has_root_name(); }
    bool has_root_name() const       { return !root_name().empty(); }
    bool has_root_directory() const  { return !root_directory().empty(); }
    bool has_relative_path() const   { return !relative_path().empty(); }
    bool has_filename() const        { return !m_path.empty(); }
    bool has_parent_path() const     { return !parent_path().empty(); }
    bool is_complete() const
    {
#   ifdef BOOST_WINDOWS_PATH
      return has_root_name() && has_root_directory();
#   else
      return has_root_directory();
#   endif
    }

    //  -----  locale  -----

    static std::locale imbue( const std::locale & loc,
                              system::error_code & ec = system::throws )
    {
      std::locale tmp;
      tmp = *detail::path_locale;
      detail::path_locale = &loc;
      return tmp;
    }

    static std::locale getloc() { return *detail::path_locale; }

    //  -----  iterators  -----

    class iterator;
    typedef iterator const_iterator;

    iterator begin() const;
    iterator end() const;

  //------------------------------------------------------------------------------------//
  //                          class path private members                                //
  //------------------------------------------------------------------------------------//

  private:

    //  m_path has the type, encoding, and format required by the native
    //  operating system. Thus for POSIX and Windows there is no conversion for
    //  passing m_path.c_str() to the O/S API or when obtaining a path from the
    //  O/S API. POSIX encoding is unspecified other than for dot and slash
    //  characters; POSIX just treats paths as a sequence of bytes. Windows
    //  encoding is UCS-2 or UTF-16 depending on the version.

    string_type  m_path;  // Windows: backslashes NOT converted to slashes

    //  These helpers factor out common code and convert iterators to pointers.
    template< class InputIterator >
    inline void m_append( InputIterator begin,
      string_type & target, system::error_code & ec )
    {
      BOOST_ASSERT( &*begin );
      path_traits::append( &*begin, target, ec );
    }

    template< class FowardIterator >
    inline void m_append( FowardIterator begin, FowardIterator end,
      string_type & target, system::error_code & ec )
    { 
      if ( begin == end ) return;
      BOOST_ASSERT( &*begin );
      path_traits::append( &*begin,
        &*begin + std::distance( begin, end ), // avoid dereference of end iterator
        target, ec );
    }

    void append_separator_if_needed_();

    // Was qualified; como433beta8 reports:
    //    warning #427-D: qualified name is not allowed in member declaration 
    friend class iterator;
    friend bool operator<( const path & lhs, const path & rhs );

    static bool m_path_lex_compare( iterator first1, iterator last1,
      iterator first2, iterator last2 );

    // see path::iterator::increment/decrement comment below
    static void m_path_iterator_increment( path::iterator & it );
    static void m_path_iterator_decrement( path::iterator & it );
  };  // class path

  //------------------------------------------------------------------------------------//
  //                            class path::iterator                                    //
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
    scoped_path_locale( const std::locale & loc,
                        system::error_code & ec = system::throws )
                      : m_saved_locale(loc)
    {
      path::imbue( loc, ec );
    }

    ~scoped_path_locale()   // never throws
    {
      system::error_code ec;
      path::imbue( m_saved_locale, ec );
    };

  private:
    std::locale m_saved_locale;
  };
   
  //------------------------------------------------------------------------------------//
  //                                                                                    //
  //                              non-member functions                                  //
  //                                                                                    //
  //------------------------------------------------------------------------------------//

  inline bool operator<( const path & lhs, const path & rhs )
  {
    // because path iterators yield paths, std::lexicographical_compare 
    // infinately recurses, so use a path aware version
    return path::m_path_lex_compare(
      lhs.begin(), lhs.end(), rhs.begin(), rhs.end() );
  }

  inline bool operator==( const path & lhs, const path & rhs ) { return !(lhs < rhs) && !(rhs < lhs); }
  inline bool operator!=( const path & lhs, const path & rhs ) { return !(lhs == rhs); }
  inline bool operator<=( const path & lhs, const path & rhs ) { return !(rhs < lhs); }
  inline bool operator> ( const path & lhs, const path & rhs ) { return rhs < lhs; }
  inline bool operator>=( const path & lhs, const path & rhs ) { return !(lhs < rhs);  }

  inline void swap( path & lhs, path & rhs )                   { lhs.swap( rhs ); }

  inline path operator/( const path & lhs, const path & rhs )  { return path( lhs ) /= rhs; }

  //  inserters and extractors

  // templated versions are too aggressive; path_test gets ambiguous overloads
  // with std::basic_string.
  //
  //template < class Ostream >
  //Ostream & operator<<( Ostream & os, const path & p )
  //{
  //  os << p.string< std::basic_string<typename Ostream::char_type,
  //                                    typename Ostream::traits_type > >();
  //  return os;
  //}
  //
  //template < class Istream >
  //Istream & operator>>( Istream & is, path & p )
  //{
  //  std::basic_string<typename Istream::char_type,
  //                    typename Istream::traits_type > > str;
  //  is >> str;
  //  p = str;
  //  return is;
  //}

  inline std::ostream & operator<<( std::ostream & os, const path & p )
  {
    os << p.string();
    return os;
  }
  
  inline std::wostream & operator<<( std::wostream & os, const path & p )
  {
    os << p.wstring();
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
