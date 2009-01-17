//  filesystem path.cpp  -------------------------------------------------------------  //

//  Copyright Beman Dawes 2008

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// define BOOST_FILESYSTEM_SOURCE so that <boost/system/config.hpp> knows
// the library is being built (possibly exporting rather than importing code)
#define BOOST_FILESYSTEM_SOURCE 

#include <boost/filesystem/config.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/scoped_array.hpp>
#include <boost/system/error_code.hpp>
#include <boost/assert.hpp>
#include <cstddef>
#include <cstring>
#include <cassert>

#ifdef BOOST_FILESYSTEM_DEBUG
# include <iostream>
# include <iomanip>
#endif

#ifdef BOOST_WINDOWS_PATH
#  include <windows.h>
#endif

namespace fs = boost::filesystem;

using fs::path;

using std::string;
using std::wstring;

using boost::system::error_code;

#ifndef BOOST_FILESYSTEM_CODECVT_BUF_SIZE
# define BOOST_FILESYSTEM_CODECVT_BUF_SIZE 256
#endif

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                                class path helpers                                    //
//                                                                                      //
//--------------------------------------------------------------------------------------//

namespace
{
  //------------------------------------------------------------------------------------//
  //                        miscellaneous class path helpers                            //
  //------------------------------------------------------------------------------------//

  typedef path::size_type    size_type;
  typedef path::string_type  string_type;
  typedef path::value_type   value_type;

  const std::size_t default_codecvt_buf_size = BOOST_FILESYSTEM_CODECVT_BUF_SIZE;

# ifdef BOOST_WINDOWS_PATH

  const wchar_t separator = L'/';
  const wchar_t preferred_separator = L'\\';
  const wchar_t * const separators = L"/\\";
  const wchar_t * separator_string = L"/";
  const wchar_t * preferred_separator_string = L"\\";
  const wchar_t colon = L':';
  const wchar_t dot = L'.';
  const fs::path dot_path( L"." );

# else

  const char separator = '/';
  const char preferred_separator = '/';
  const char * const separators = "/";
  const char * separator_string = "/";
  const char * preferred_separator_string = "/";
  const char colon = ':';
  const char dot = '.';
  const fs::path dot_path( "." );

# endif

  inline bool is_separator( fs::path::value_type c )
  {
    return c == separator
#     ifdef BOOST_WINDOWS_PATH
      || c == preferred_separator
#     endif
      ;
  }

  bool is_non_root_separator( const string_type & str, size_type pos );
    // pos is position of the separator

  size_type filename_pos( const string_type & str,
                          size_type end_pos ); // end_pos is past-the-end position
  //  Returns: 0 if str itself is filename (or empty)

  size_type root_directory_start( const string_type & path, size_type size );
  //  Returns:  npos if no root_directory found

  void first_element(
      const string_type & src,
      size_type & element_pos,
      size_type & element_size,
#    if !BOOST_WORKAROUND( BOOST_MSVC, <= 1310 ) // VC++ 7.1
      size_type size = string_type::npos
#    else
      size_type size = -1
#    endif
    );

}  // unnamed namespace

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                            class path implementation                                 //
//                                                                                      //
//--------------------------------------------------------------------------------------//

namespace boost
{
namespace filesystem
{

# ifdef BOOST_WINDOWS_PATH

  //  generic  ---------------------------------------------------------------//

  const path path::generic() const
  {
    path tmp;

    for ( string_type::const_iterator it = m_path.begin();
          it != m_path.end(); ++it )
    {
      tmp.m_path += *it != L'\\' ? *it : L'/';
    }

    return tmp;
  }
  
  //  native  ---------------------------------------------------------------//

  const path path::native() const
  {
    path tmp;

    for ( string_type::const_iterator it = m_path.begin();
          it != m_path.end(); ++it )
    {
      tmp.m_path += *it != L'/' ? *it : L'\\';
    }

    return tmp;
  }

# endif  // BOOST_WINDOWS_PATH

  //  append_separator_if_needed_  -------------------------------------------//

  void path::append_separator_if_needed_()
  {
    if ( !m_path.empty() &&
#   ifdef BOOST_WINDOWS_PATH
      *(m_path.end()-1) != colon && 
#   endif
      !is_separator( *(m_path.end()-1) ) )
    {
      m_path += preferred_separator;
    }
  }

  //  decomposition  ---------------------------------------------------------//

  path  path::root_path() const
  { 
    path temp( root_name() );
    if ( !root_directory().empty() ) temp.m_path += root_directory().c_str();
    return temp;
  } 

  path path::root_name() const
  {
    iterator itr( begin() );

    return ( itr.m_pos != m_path.size()
      && (
          ( itr.m_element.m_path.size() > 1
            && is_separator( itr.m_element.m_path[0] )
            && is_separator( itr.m_element.m_path[1] )
          )
#       ifdef BOOST_WINDOWS_PATH
        || itr.m_element.m_path[itr.m_element.m_path.size()-1] == colon
#       endif
         ) )
      ? itr.m_element
      : path();
  }

  path path::root_directory() const
  {
    size_type pos( root_directory_start( m_path, m_path.size() ) );

    return pos == string_type::npos
      ? path()
      : path( m_path.c_str() + pos, m_path.c_str() + pos + 1 );
  }

  path path::relative_path() const
  {
    iterator itr( begin() );

    for ( ; itr.m_pos != m_path.size()
      && ( is_separator( itr.m_element.m_path[0] )
#       ifdef BOOST_WINDOWS_PATH
      || itr.m_element.m_path[itr.m_element.m_path.size()-1] == colon
#       endif
           ); ++itr ) {}

    return path( m_path.c_str() + itr.m_pos );
  }

  path path::parent_path() const
  {
    size_type end_pos( filename_pos( m_path, m_path.size() ) );

    bool filename_was_separator( m_path.size()
      && is_separator( m_path[end_pos] ) );

    // skip separators unless root directory
    size_type root_dir_pos( root_directory_start( m_path, end_pos ) );
    for ( ; 
      end_pos > 0
      && (end_pos-1) != root_dir_pos
      && is_separator( m_path[end_pos-1] )
      ;
      --end_pos ) {}

   return (end_pos == 1 && root_dir_pos == 0 && filename_was_separator)
     ? path()
     : path( m_path.c_str(), m_path.c_str() + end_pos );
  }

  path path::filename() const
  {
    size_type pos( filename_pos( m_path, m_path.size() ) );
    return (m_path.size()
              && pos
              && is_separator( m_path[pos] )
              && is_non_root_separator(m_path, pos))
      ? dot_path
      : path( m_path.c_str() + pos );
  }

  path path::stem() const
  {
    path name( filename() );
    size_type pos( name.m_path.rfind( dot ) );
    return pos == string_type::npos
      ? name
      : path( name.m_path.c_str(), name.m_path.c_str() + pos );
  }

  path path::extension() const
  {
    path name( filename() );
    size_type pos( name.m_path.rfind( dot ) );
    return pos == string_type::npos
      ? path()
      : path( name.m_path.c_str() + pos );
  }

  path & path::remove_filename()
  {
    m_path.erase( filename_pos( m_path, m_path.size() ) );
    return *this;
  }

}  // namespace filesystem
}  // namespace boost
  
//--------------------------------------------------------------------------------------//
//                                                                                      //
//                         class path helpers implementation                            //
//                                                                                      //
//--------------------------------------------------------------------------------------//

namespace
{

  //  is_non_root_separator  -------------------------------------------------//

  bool is_non_root_separator( const string_type & str, size_type pos )
    // pos is position of the separator
  {
    BOOST_ASSERT( !str.empty() && is_separator( str[pos] )
      && "precondition violation" );

    // subsequent logic expects pos to be for leftmost slash of a set
    while ( pos > 0 && is_separator( str[pos-1] ) )
      --pos;

    return  pos != 0
      && (pos <= 2 || !is_separator( str[1] )
        || str.find_first_of( separators, 2 ) != pos)
#   ifdef BOOST_WINDOWS_PATH
      && (pos !=2 || str[1] != colon)
#   endif
        ;
  }

  //  filename_pos  ----------------------------------------------------------//

  size_type filename_pos( const string_type & str,
                          size_type end_pos ) // end_pos is past-the-end position
    // return 0 if str itself is filename (or empty)
  {
    // case: "//"
    if ( end_pos == 2 
      && is_separator( str[0] )
      && is_separator( str[1] ) ) return 0;

    // case: ends in "/"
    if ( end_pos && is_separator( str[end_pos-1] ) )
      return end_pos-1;
    
    // set pos to start of last element
    size_type pos( str.find_last_of( separators, end_pos-1 ) );

# ifdef BOOST_WINDOWS_PATH
    if ( pos == string_type::npos )
      pos = str.find_last_of( colon, end_pos-2 );
# endif

    return ( pos == string_type::npos // path itself must be a filename (or empty)
      || (pos == 1 && is_separator( str[0] )) ) // or net
        ? 0 // so filename is entire string
        : pos + 1; // or starts after delimiter
  }

  //  root_directory_start  --------------------------------------------------//

  size_type root_directory_start( const string_type & path, size_type size )
  // return npos if no root_directory found
  {

#   ifdef BOOST_WINDOWS_PATH
    // case "c:/"
    if ( size > 2
      && path[1] == colon
      && is_separator( path[2] ) ) return 2;
#   endif

    // case "//"
    if ( size == 2
      && is_separator( path[0] )
      && is_separator( path[1] ) ) return string_type::npos;

    // case "//net {/}"
    if ( size > 3
      && is_separator( path[0] )
      && is_separator( path[1] )
      && !is_separator( path[2] ) )
    {
      string_type::size_type pos( path.find_first_of( separators, 2 ) );
      return pos < size ? pos : string_type::npos;
    }
    
    // case "/"
    if ( size > 0 && is_separator( path[0] ) ) return 0;

    return string_type::npos;
  }

  //  first_element ----------------------------------------------------------//
  //   sets pos and len of first element, excluding extra separators
  //   if src.empty(), sets pos,len, to 0,0.

  void first_element(
      const string_type & src,
      size_type & element_pos,
      size_type & element_size,
      size_type size
      )
  {
    if ( size == string_type::npos ) size = src.size();
    element_pos = 0;
    element_size = 0;
    if ( src.empty() ) return;

    string_type::size_type cur(0);
    
    // deal with // [network]
    if ( size >= 2 && is_separator( src[0] )
      && is_separator( src[1] )
      && (size == 2
        || !is_separator( src[2] )) )
    { 
      cur += 2;
      element_size += 2;
    }

    // leading (not non-network) separator
    else if ( is_separator( src[0] ) )
    {
      ++element_size;
      // bypass extra leading separators
      while ( cur+1 < size
        && is_separator( src[cur+1] ) )
      {
        ++cur;
        ++element_pos;
      }
      return;
    }

    // at this point, we have either a plain name, a network name,
    // or (on Windows only) a device name

    // find the end
    while ( cur < size
#     ifdef BOOST_WINDOWS_PATH
      && src[cur] != colon
#     endif
      && !is_separator( src[cur] ) )
    {
      ++cur;
      ++element_size;
    }

#   ifdef BOOST_WINDOWS_PATH
    if ( cur == size ) return;
    // include device delimiter
    if ( src[cur] == colon )
      { ++element_size; }
#   endif

    return;
  }

}  // unnammed namespace

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                        class path::iterator implementation                           //
//                                                                                      //
//--------------------------------------------------------------------------------------//

namespace boost
{
namespace filesystem
{

  path::iterator path::begin() const
  {
    iterator itr;
    itr.m_path_ptr = this;
    path::size_type element_size;
    first_element( m_path, itr.m_pos, element_size );
    itr.m_element = m_path.substr( itr.m_pos, element_size );
    if ( itr.m_element.m_path == preferred_separator_string )
      itr.m_element.m_path = separator_string;  // needed for Windows, harmless on POSIX
    return itr;
  }

  path::iterator path::end() const
  {
    iterator itr;
    itr.m_path_ptr = this;
    itr.m_pos = m_path.size();
    return itr;
  }

  void path::m_path_iterator_increment( path::iterator & it )
  {
    BOOST_ASSERT( it.m_pos < it.m_path_ptr->m_path.size() && "path::basic_iterator increment past end()" );

    // increment to position past current element
    it.m_pos += it.m_element.m_path.size();

    // if end reached, create end basic_iterator
    if ( it.m_pos == it.m_path_ptr->m_path.size() )
    {
      it.m_element.clear(); 
      return;
    }

    // both POSIX and Windows treat paths that begin with exactly two separators specially
    bool was_net( it.m_element.m_path.size() > 2
      && is_separator( it.m_element.m_path[0] )
      && is_separator( it.m_element.m_path[1] )
      && !is_separator( it.m_element.m_path[2] ) );

    // process separator (Windows drive spec is only case not a separator)
    if ( is_separator( it.m_path_ptr->m_path[it.m_pos] ) )
    {
      // detect root directory
      if ( was_net
#       ifdef BOOST_WINDOWS_PATH
        // case "c:/"
        || it.m_element.m_path[it.m_element.m_path.size()-1] == colon
#       endif
         )
      {
        it.m_element.m_path = separator;
        return;
      }

      // bypass separators
      while ( it.m_pos != it.m_path_ptr->m_path.size()
        && is_separator( it.m_path_ptr->m_path[it.m_pos] ) )
        { ++it.m_pos; }

      // detect trailing separator, and treat it as ".", per POSIX spec
      if ( it.m_pos == it.m_path_ptr->m_path.size()
        && is_non_root_separator( it.m_path_ptr->m_path, it.m_pos-1 ) ) 
      {
        --it.m_pos;
        it.m_element = dot_path;
        return;
      }
    }

    // get next element
    size_type end_pos( it.m_path_ptr->m_path.find_first_of( separators, it.m_pos ) );
    if ( end_pos == string_type::npos ) end_pos = it.m_path_ptr->m_path.size();
    it.m_element = it.m_path_ptr->m_path.substr( it.m_pos, end_pos - it.m_pos );
  }

  void path::m_path_iterator_decrement( path::iterator & it )
  {
    BOOST_ASSERT( it.m_pos && "path::iterator decrement past begin()"  );

    size_type end_pos( it.m_pos );

    // if at end and there was a trailing non-root '/', return "."
    if ( it.m_pos == it.m_path_ptr->m_path.size()
      && it.m_path_ptr->m_path.size() > 1
      && is_separator( it.m_path_ptr->m_path[it.m_pos-1] )
      && is_non_root_separator( it.m_path_ptr->m_path, it.m_pos-1 ) 
       )
    {
      --it.m_pos;
      it.m_element = dot_path;
      return;
    }

    size_type root_dir_pos( root_directory_start( it.m_path_ptr->m_path, end_pos ) );

    // skip separators unless root directory
    for ( 
      ; 
      end_pos > 0
      && (end_pos-1) != root_dir_pos
      && is_separator( it.m_path_ptr->m_path[end_pos-1] )
      ;
      --end_pos ) {}

    it.m_pos = filename_pos( it.m_path_ptr->m_path, end_pos );
    it.m_element = it.m_path_ptr->m_path.substr( it.m_pos, end_pos - it.m_pos );
    if ( it.m_element.m_path == preferred_separator_string )
      it.m_element.m_path = separator_string;  // needed for Windows, harmless on POSIX
  }

  bool path::m_path_lex_compare( iterator first1, iterator last1,
    iterator first2, iterator last2 )
  {
    for ( ; first1 != last1 && first2 != last2 ; ++first1, ++first2)
    {
      if ( first1->m_path < first2->m_path ) return true;
      if ( first2->m_path < first1->m_path ) return false;
    }
    return first1 == last1 && first2 != last2;
  }

}  // namespace filesystem
}  // namespace boost

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                                 detail helpers                                       //
//                                                                                      //
//--------------------------------------------------------------------------------------//

namespace
{
# ifdef BOOST_WINDOWS_API

  //------------------------------------------------------------------------------------//
  //                                                                                    //
  //                     class windows_file_api_codecvt_facet                           //
  //                                                                                    //
  //  Warning: partial implementation; even do_in and do_out only partially meet the    //
  //  standard library specifications; the "to" buffer must hold the entire result.     //
  //------------------------------------------------------------------------------------//

  class windows_file_api_codecvt_facet
    : public std::codecvt< wchar_t, char, std::mbstate_t >  
  {
  public:
    explicit windows_file_api_codecvt_facet()
        : std::codecvt<wchar_t, char, std::mbstate_t>(0) {}
  protected:

    virtual bool do_always_noconv() const throw() { return false; }

    //  seems safest to assume variable number of characters since we don't
    //  actually know what codepage is active
    virtual int do_encoding() const throw() { return 0; }

    virtual std::codecvt_base::result do_in( std::mbstate_t& state, 
      const char * from, const char * from_end, const char *& from_next,
      wchar_t * to, wchar_t * to_end, wchar_t *& to_next ) const;

    virtual std::codecvt_base::result do_out( std::mbstate_t & state,
      const wchar_t * from, const wchar_t * from_end, const wchar_t *& from_next,
      char * to, char * to_end, char *& to_next ) const;

    virtual std::codecvt_base::result do_unshift( std::mbstate_t&,
        char * from, char * /*to*/, char * & next) const  { return ok; } 

    virtual int do_length( std::mbstate_t &,
      const char * from, const char * from_end, std::size_t max ) const  { return 0; }

    virtual int do_max_length() const throw () { return 0; }
  };

  std::codecvt_base::result windows_file_api_codecvt_facet::do_in(
    std::mbstate_t & state, 
    const char * from, const char * from_end, const char *& from_next,
    wchar_t * to, wchar_t * to_end, wchar_t *& to_next ) const
  {
    UINT codepage = AreFileApisANSI() ? CP_THREAD_ACP : CP_OEMCP;

    int count;
    if ( (count = ::MultiByteToWideChar( codepage, MB_PRECOMPOSED, from,
      from_end - from, to, to_end - to )) == 0 ) 
    {
      return error;  // conversion failed
    }

    from_next = from_end;
    to_next = to + count;
    *to_next = L'\0';
    return ok;
 }

  std::codecvt_base::result windows_file_api_codecvt_facet::do_out(
    std::mbstate_t & state,
    const wchar_t * from, const wchar_t * from_end, const wchar_t*  & from_next,
    char * to, char * to_end, char * & to_next ) const
  {
    UINT codepage = AreFileApisANSI() ? CP_THREAD_ACP : CP_OEMCP;

    int count;
    if ( (count = ::WideCharToMultiByte( codepage, WC_NO_BEST_FIT_CHARS, from,
      from_end - from, to, to_end - to, 0, 0 )) == 0 )
    {
      return error;  // conversion failed
    }

    from_next = from_end;
    to_next = to + count;
    *to_next = '\0';
    return ok;
  }
# endif  // BOOST_WINDOWS_API

  //------------------------------------------------------------------------------------//
  //                              locale helpers                                        //
  //------------------------------------------------------------------------------------//

  // std::locale construction can throw (if LC_MESSAGES is wrong, for example),
  // so a static at function scope is used to ensure that exceptions can be
  // caught. (A previous version was at namespace scope, so initialization
  // occurred before main(), preventing exceptions from being caught.)

  std::locale default_locale()
  {
# ifdef BOOST_WINDOWS_API
    std::locale global_loc = std::locale();
    std::locale loc( global_loc, new windows_file_api_codecvt_facet );
    return loc;
# else
    // ISO C calls this "the locale-specific native environment":
    return std::locale("");
# endif
  }

  std::locale & path_locale()
  {
    // ISO C calls this "the locale-specific native environment":
    static std::locale loc( default_locale() );
    return loc;
  }

  const std::codecvt<wchar_t, char, std::mbstate_t> *&
  wchar_t_codecvt_facet()
  {
   static const std::codecvt<wchar_t, char, std::mbstate_t> *
     facet(
       &std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t> >
        ( path_locale() ) );
   return facet;
  }

}  // unnamed namespace

//--------------------------------------------------------------------------------------//
//                           path::imbue implementation                                 //
//--------------------------------------------------------------------------------------//

namespace boost
{
namespace filesystem
{

  std::locale path::imbue( const std::locale & loc )
  {
    std::locale temp( path_locale() );
    path_locale() = loc;
    wchar_t_codecvt_facet() = &std::use_facet
        <std::codecvt<wchar_t, char, std::mbstate_t> >( path_locale() );
    return temp;
  }

  std::locale path::imbue( const std::locale & loc, system::error_code & ec )
  {
    try
    {
      std::locale temp( path_locale() );
      path_locale() = loc;
      wchar_t_codecvt_facet() = &std::use_facet
          <std::codecvt<wchar_t, char, std::mbstate_t> >( path_locale() );
      ec.clear();
      return temp;
    }
    catch (...)
    {
      assert( 0 && "not implemented yet" );  // TODO
    }
  }

//--------------------------------------------------------------------------------------//
//                              detail implementation                                   //
//--------------------------------------------------------------------------------------//

namespace detail
{
# ifdef BOOST_WINDOWS_API
#  define APPEND_DIRECTION in
#  define CONVERT_DIRECTION out
# else
#  define APPEND_DIRECTION out
#  define CONVERT_DIRECTION in
# endif

  //------------------------------------------------------------------------------------//
  //                                   append                                           //
  //------------------------------------------------------------------------------------//

  //  actual append done here to factor it out from messy buffer management code;
  //  this function just assumes the buffer is large enough.
  inline void do_append(
                   const interface_value_type * from, const interface_value_type * from_end,
                   value_type * to, value_type * to_end,
                   string_type & target, error_code & ec )
  {
    //std::cout << std::hex
    //          << " from=" << std::size_t(from)
    //          << " from_end=" << std::size_t(from_end)
    //          << " to=" << std::size_t(to)
    //          << " to_end=" << std::size_t(to_end)
    //          << std::endl;

    std::mbstate_t state  = std::mbstate_t();  // perhaps unneeded, but cuts bug reports
    const interface_value_type * from_next;
    value_type * to_next;

    std::codecvt_base::result res;

    if ( (res=wchar_t_codecvt_facet()->APPEND_DIRECTION( state, from, from_end, from_next,
           to, to_end, to_next )) != std::codecvt_base::ok )
    {
      //std::cout << " result is " << static_cast<int>(res) << std::endl;
      assert( 0 && "append error handling not implemented yet" );
      throw "append error handling not implemented yet";
    }
    if ( &ec != &system::throws ) ec.clear();
    target.append( to, to_next ); 
  }

  BOOST_FILESYSTEM_DECL
  void append( const interface_value_type * begin, const interface_value_type * end,
  string_type & target, error_code & ec )
  {
    if ( !end )
    {
      // compute strlen by hand since interface_value_type may not be char
      end = begin;
      while (*end) ++end;
    }

    if ( begin == end )
    {
      if ( &ec != &system::throws ) ec.clear();
      return;
    }

    std::size_t buf_size = (end - begin) * 3;  // perhaps too large, but that's OK

    //  dynamically allocate a buffer only if source is unusually large
    if ( buf_size > default_codecvt_buf_size )
    {
      boost::scoped_array< value_type > buf( new value_type [buf_size] );
      do_append( begin, end, buf.get(), buf.get()+buf_size, target, ec );
    }
    else
    {
      value_type buf[default_codecvt_buf_size];
      do_append( begin, end, buf, buf+default_codecvt_buf_size, target, ec );
    }
  }

  //------------------------------------------------------------------------------------//
  //                                  convert                                           //
  //------------------------------------------------------------------------------------//

  //  actual convert done here to factor it out from messy buffer management code;
  //  this function just assumes the buffer is large enough.
  inline interface_string_type  do_convert(
    const value_type * from, const value_type * from_end,
    interface_value_type * to, interface_value_type * to_end,
    error_code & ec )
  {
    std::mbstate_t state  = std::mbstate_t();  // perhaps unneeded, but cuts bug reports
    const value_type * from_next;
    interface_value_type * to_next;

    if ( wchar_t_codecvt_facet()->CONVERT_DIRECTION( state, from, from_end,
          from_next, to, to_end, to_next ) != std::codecvt_base::ok )
    {
      assert( 0 && "convert error handling not implemented yet" );
      throw "convert error handling not implemented yet";
    }
    if ( &ec != &system::throws ) ec.clear();
    return interface_string_type( to, to_next ); 
  }

  BOOST_FILESYSTEM_DECL
  interface_string_type convert_to_string( const string_type & src, error_code & ec )
  {
    if ( src.empty() )
    {
      if ( &ec != &system::throws ) ec.clear();
      return interface_string_type();
    }

    //  The codecvt length functions may not be implemented, and I don't really
    //  understand them either. Thus this code is just a guess; if it turns
    //  out the buffer is too small then an error will be reported and the code
    //  will have to be fixed.
    std::size_t buf_size = src.size() * 4;
    buf_size += 4;  // encodings like shift-JIS need some prefix space

    //  dynamically allocate a buffer only if source is unusually large
    if ( buf_size > default_codecvt_buf_size )
    {
      boost::scoped_array< interface_value_type > buf( new interface_value_type [buf_size] );
      return do_convert( src.c_str(), src.c_str()+src.size(),
        buf.get(), buf.get()+buf_size, ec );
    }
    else
    {
      interface_value_type buf[default_codecvt_buf_size];
      return do_convert( src.c_str(), src.c_str()+src.size(), buf, buf+buf_size, ec );
    }
  }

}  // namespace detail
}  // namespace filesystem
}  // namespace boost
