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
#include <windows.h>

namespace fs = boost::filesystem;

using fs::path;

using std::string;
using std::wstring;

using boost::system::error_code;

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                                class path helpers                                    //
//                                                                                      //
//--------------------------------------------------------------------------------------//

namespace
{

  typedef path::size_type    size_type;
  typedef path::string_type  string_type;
  typedef path::value_type   value_type;

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
//                                   class path                                         //
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

# else   // BOOST_POSIX_API
  ...
# endif  // BOOST_POSIX_API

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
//                          class path helper implementation                            //
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
//                               class path::iterator                                   //
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
//                                  path_traits                                         //
//                                                                                      //
//--------------------------------------------------------------------------------------//

namespace
{

# ifdef BOOST_WINDOWS_API

  const std::locale windows_default;  // only use is to take address

  void windows_append( const char * begin, const char * end,
                                 // end == 0 for null terminated MBCS
                                 wstring & target, error_code & ec );

  void locale_append( const char * begin, const char * end,
                                // end == 0 for null terminated MBCS
                              wstring & target, error_code & ec );
# else   // BOOST_POSIX_API

  ...

# endif

}  // unnamed namespace

namespace boost
{
namespace filesystem
{

namespace detail
{
  const std::locale * path_locale( &windows_default );

  //------------------------------------------------------------------------------------//
  //                                   append                                           //
  //------------------------------------------------------------------------------------//

  void append( const char * begin, const char * end,
                       wstring & target, error_code & ec )
  {
    if ( path_locale == &windows_default )
      windows_append( begin, end, target, ec );
    else
      locale_append( begin, end, target, ec );
  }

  //------------------------------------------------------------------------------------//
  //                                  convert                                           //
  //------------------------------------------------------------------------------------//

  BOOST_FILESYSTEM_DECL
  string convert_to_string( const wstring & src, error_code & ec )
  {
    if ( src.empty() ) return std::string();

    UINT codepage = AreFileApisANSI() ? CP_THREAD_ACP : CP_OEMCP;

    //  buffer management strategy; use a probably large enough default buffer,
    //  but fallback to an explict allocation if the default buffer is too small

    const std::size_t default_buf_sz = MAX_PATH+1;
    char buf[default_buf_sz+1];
    int count;

    if ( (count = ::WideCharToMultiByte( codepage, WC_NO_BEST_FIT_CHARS, src.c_str(),
      src.size(), buf, default_buf_sz, 0, 0 )) != 0 ) // success
    {
      ec.clear();
      buf[count] = '\0';
      return std::string( buf );
    }

    // TODO: implement fallback
    BOOST_ASSERT(0);
    throw "path::native_string() error handling not implemented yet";
    return std::string();
  }

}  // namespace detail
}  // namespace filesystem
}  // namespace boost

namespace
{

  void locale_append( const char * begin, const char * end,
                                // end == 0 for null terminated MBCS
                                 wstring & target,  error_code & ec )
  {
    BOOST_ASSERT( 0 && "not implemented yet" );
  }


  void windows_append( const char * begin, const char * end,
     wstring & target,  error_code & ec )
  {
    UINT codepage = AreFileApisANSI() ? CP_THREAD_ACP : CP_OEMCP;
    int size( end == 0 ? -1 : (end - begin) );

    //  buffer management:
    //
    //    if path prefixed by "\\?\" (Windows 'long path' prefix)
    //       then use dynamic allocation, else use automatic allocation

    bool dynamic_allocation = target.find( L"\\\\?\\" ) == 0
      || ( target.empty()
      && ( (size >= 4 && std::memcmp( begin, "\\\\?\\", 4 ) == 0)
      || (size == -1 && std::strcmp( begin, "\\\\?\\" ) == 0) ) );

    wchar_t                         stack_buf[MAX_PATH+1];
    boost::scoped_array< wchar_t >  heap_buf;
    wchar_t *                       buf = stack_buf;
    int                             buf_size = sizeof(stack_buf)/sizeof(wchar_t) - 1;      

    if ( dynamic_allocation )
    {
      //  get the allocation size for the buffer
      //     rationale for calling MultiByteToWideChar: begin may point to a
      //     multi-byte encoded string with embedded nulls, so a simplistic
      //     computation of the size can fail
      
      buf_size = ::MultiByteToWideChar( codepage, MB_PRECOMPOSED, begin, size, 0, 0 );
      heap_buf.reset( new wchar_t [buf_size] );
      buf = heap_buf.get();
    }

    //  perform the conversion

    if ( (buf_size = ::MultiByteToWideChar( codepage, MB_PRECOMPOSED, begin,
      size, buf, buf_size )) == 0 ) 
    {
      // conversion failed
      ec =  error_code( ::GetLastError(), boost::system::system_category );
      return;
    }

    ec.clear();

    //  perform the append

    buf[buf_size] = L'\0';
    target += buf;
  }

}  // unnamed namespace

