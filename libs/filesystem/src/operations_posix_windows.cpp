//  directory_posix_windows.cpp  ---------------------------------------------//

// < ----------------------------------------------------------------------- > 
// <   Copyright � 2002 Beman Dawes.                                         >
// <   Copyright � 2001 Dietmar K�hl, All Rights Reserved                    >
// <                                                                         > 
// <   Permission to use, copy, modify, distribute and sell this             > 
// <   software for any purpose is hereby granted without fee, provided      > 
// <   that the above copyright notice appears in all copies and that        > 
// <   both that copyright notice and this permission notice appear in       > 
// <   supporting documentation. The authors make no representations about   > 
// <   the suitability of this software for any purpose. It is provided      > 
// <   "as is" without expressed or implied warranty.                        > 
// < ----------------------------------------------------------------------- > 

//  See http://www.boost.org for most recent version including documentation.

//----------------------------------------------------------------------------//


//  The point of this implementation is to prove the interface.  There is no
//  claim the implementation is efficient, follows good coding practice, etc.


//----------------------------------------------------------------------------// 

#include "boost/config.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/exception.hpp"

namespace fs = boost::filesystem;

#if defined( BOOST_POSIX )

# include "sys/stat.h"
# include "dirent.h"
# include "unistd.h"
# include "fcntl.h"

#elif defined( BOOST_WINDOWS )

# include "windows.h"

#else
# error macro BOOST_POSIX or BOOST_WINDOWS must be defined
#endif

#include <string>
#include <cstdio>
#include <cerrno>
#include <cassert>

//  helpers  -----------------------------------------------------------------//

namespace
{
#ifdef BOOST_POSIX

# define BOOST_HANDLE DIR *
# define BOOST_INVALID_HANDLE_VALUE 0
# define BOOST_SYSTEM_DIRECTORY_TYPE struct dirent *

  inline const char *  find_first_file( const char * dir,
    BOOST_HANDLE & handle, BOOST_SYSTEM_DIRECTORY_TYPE & )
  // Returns: 0 if error, otherwise name
  {
    const char * dummy_first_name = ".";
    return ( (handle = ::opendir( dir ))
      == BOOST_INVALID_HANDLE_VALUE ) ? 0 : dummy_first_name;
  }  

  inline void find_close( BOOST_HANDLE handle ) { ::closedir( handle ); }

  inline const char * find_next_file(
    BOOST_HANDLE handle, BOOST_SYSTEM_DIRECTORY_TYPE & )
  // Returns: if EOF 0, otherwise name
  // Throws: if system reports error
  {

//  TODO: use readdir_r() if available, so code is multi-thread safe.
//  Fly-in-ointment: not all platforms support readdir_r().

    struct dirent * dp;
    errno = 0;
    if ( (dp = ::readdir( handle )) == 0 )
    {
      if ( errno != 0 )
      {
        throw fs::filesystem_error( "directory_iterator ++() failure" );
      }
      else // eof
      {
        find_close( handle );
        return 0;
      }
    }
    return dp->d_name;
  }
#else // BOOST_WINDOWS

# define BOOST_HANDLE HANDLE
# define BOOST_INVALID_HANDLE_VALUE INVALID_HANDLE_VALUE
# define BOOST_SYSTEM_DIRECTORY_TYPE WIN32_FIND_DATA

  inline const char *  find_first_file( const char * dir,
    BOOST_HANDLE & handle, BOOST_SYSTEM_DIRECTORY_TYPE & data )
  // Returns: 0 if error, otherwise name
  {
    std::string dirpath( std::string(dir) + "/*" );
    return ( (handle = FindFirstFile( dirpath.c_str(), &data ))
      == BOOST_INVALID_HANDLE_VALUE ) ? 0 : data.cFileName;
  }  

  inline void find_close( BOOST_HANDLE handle ) { FindClose( handle ); }

  inline const char * find_next_file(
    BOOST_HANDLE handle, BOOST_SYSTEM_DIRECTORY_TYPE & data )
  // Returns: 0 if EOF, otherwise name
  // Throws: if system reports error
  {
    if ( FindNextFile( handle, &data ) == 0 )
    {
      if ( GetLastError() != ERROR_NO_MORE_FILES )
      {
        throw fs::filesystem_error(
          "directory_iterator ++() failure", fs::system_error );
      }
      else // eof
      {
        find_close( handle );
        return 0;
      }
    }
    return data.cFileName;
  }

#endif

  fs::directory_iterator end_itr;

  bool empty_directory( const fs::path & dir_path )
  {
    return fs::directory_iterator(dir_path) == end_itr;
  }

  unsigned long remove_all_aux( const fs::path & any_path )
  {
    unsigned long count = 1;
    if ( fs::directory( any_path ) )
    {
      for ( boost::filesystem::directory_iterator itr( any_path );
            itr != end_itr; ++itr )
      {
        count += remove_all_aux( *itr );
      }
    }
    fs::remove( any_path );
    return count;
  }

} // unnamed namespace

//  path_descriptor_imp  -----------------------------------------------------// 

namespace boost
{
  namespace filesystem
  {

//  directory_iterator_imp  --------------------------------------------------// 

    namespace detail
    {
      class directory_iterator_imp
      {
      public:
        path              entry_path;
        BOOST_HANDLE      handle;
      };

    } // namespace detail

//  directory_iterator implementation  ---------------------------------------//

    // default ctor creates the "end" iterator (by letting m_imp default to 0)
    directory_iterator::directory_iterator() {}

    directory_iterator::directory_iterator( const path & dir_path )
      : m_imp( new detail::directory_iterator_imp )
    {
      BOOST_SYSTEM_DIRECTORY_TYPE scratch;
      const char * name;
      if ( dir_path.empty() )
       m_imp->handle = BOOST_INVALID_HANDLE_VALUE;
      else
        name = find_first_file( dir_path.directory_c_str(),
          m_imp->handle, scratch );  // sets handle

 		  if ( m_imp->handle != BOOST_INVALID_HANDLE_VALUE )
      {
        m_imp->entry_path = dir_path;
        m_imp->entry_path.m_path_append( name, path::nocheck );
		    while ( m_imp.get()
             && ( m_imp->entry_path.leaf() == "."
              || m_imp->entry_path.leaf() == ".." ) )
		      { operator++(); }
      }
      else
      {
        throw filesystem_error( std::string( 
          "directory_iterator constructor failure: " )
          + dir_path.directory_c_str(), system_error );
      }  
    }

    directory_iterator::~directory_iterator()
    {
      if ( m_imp.get() && m_imp->handle != BOOST_INVALID_HANDLE_VALUE)
        find_close( m_imp->handle );
    }

    bool directory_iterator::operator== (
      directory_iterator const& itr ) const
    {
      return m_imp == itr.m_imp;
    }

    path const & directory_iterator::operator* () const
    {
      assert( m_imp.get() ); // fails if dereference end iterator
      return m_imp->entry_path;
    }

    directory_iterator & directory_iterator::operator++ ()
    {
      assert( m_imp.get() ); // fails on increment end iterator
      assert( m_imp->handle != BOOST_INVALID_HANDLE_VALUE ); // imp reality check

      BOOST_SYSTEM_DIRECTORY_TYPE scratch;
      const char * name;
      if ( (name = find_next_file( m_imp->handle, scratch )) != 0 )
      {
        m_imp->entry_path.m_replace_leaf( name );
      }
      else {
        m_imp.reset(); // make m_imp the end iterator
      }
      return *this;
    }

//  free functions  ----------------------------------------------------------//

    bool exists( const path & any_path )
    {
#   ifdef BOOST_POSIX
      struct ::stat path_stat;
      return ::stat( any_path.file_c_str(), &path_stat ) == 0;  
#   else
      return GetFileAttributes( any_path.file_c_str() ) != 0xFFFFFFFF;
#   endif
    }

    bool directory( const path & any_path )
    {
#   ifdef BOOST_POSIX
      struct ::stat path_stat;
      if ( ::stat( any_path.directory_c_str(), &path_stat ) != 0 )
        throw filesystem_error( std::string("is_directory(): ")
          + any_path.directory_c_str(), system_error );
      return S_ISDIR( path_stat.st_mode );
#   else
      DWORD attributes = GetFileAttributes( any_path.directory_c_str() );
      if ( attributes == 0xFFFFFFFF )
        throw filesystem_error( std::string("is_directory(): ")
          + any_path.directory_c_str(), system_error );
      return (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
#   endif
    }

    bool empty( const path & any_path )
    {
#   ifdef BOOST_POSIX
      struct ::stat path_stat;
      if ( ::stat( any_path.file_c_str(), &path_stat ) != 0 )
              throw filesystem_error( std::string("empty(): ")
                + any_path.file_c_str(), system_error );
      
      return S_ISDIR( path_stat.st_mode )
        ? empty_directory( any_path )
        : path_stat.st_size == 0;
#   else
      WIN32_FILE_ATTRIBUTE_DATA fad;
      if ( !GetFileAttributesEx( any_path.file_c_str(),
              GetFileExInfoStandard, &fad ) )
              throw filesystem_error( std::string("empty(): ")
                + any_path.file_c_str(), system_error );
      
      return ( fad.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        ? empty_directory( any_path )
        :( !fad.nFileSizeHigh && !fad.nFileSizeLow );
#   endif
    }

    void create_directory( const path & dir_path )
    {
#   ifdef BOOST_POSIX
      if ( ::mkdir( dir_path.directory_c_str(),
        S_IRWXU|S_IRWXG|S_IRWXO ) !=0 )
#   else
      if ( !CreateDirectory( dir_path.directory_c_str(), 0 ) )
#   endif
        throw filesystem_error( std::string("create_directory(): ")
          + dir_path.directory_c_str(), system_error );
    }

    void remove( const path & any_path )
    {
      if ( !exists( any_path ) )
        throw fs::filesystem_error( std::string("remove() on: ")
          + any_path.file_c_str(), system_error );
      if ( directory( any_path ) )
      {
#   ifdef BOOST_POSIX
        if ( ::rmdir( any_path.file_c_str() ) != 0 )
#   else
        if ( !RemoveDirectory( any_path.file_c_str() ) )
#   endif
          throw fs::filesystem_error( std::string("remove() on: ")
            + any_path.file_c_str(), system_error );
      }
      else
      {
#   ifdef BOOST_POSIX
        if ( ::remove( any_path.file_c_str() ) != 0 )
#   else
        if ( !DeleteFile( any_path.file_c_str() ) )
#   endif
          throw fs::filesystem_error( std::string("remove() on: ")
            + any_path.file_c_str(), system_error );
      }
    }

    unsigned long remove_all( const path & any_path )
    {
      return exists( any_path ) ? remove_all_aux( any_path ) : 0;
    }

    void rename( const path & old_path,
                 const path & new_path )
    {
#   ifdef BOOST_POSIX
      if ( exists( new_path ) // POSIX is too permissive so must check
        || ::rename( old_path.file_c_str(), new_path.file_c_str() ) != 0 )
#   else
      if ( !MoveFile( old_path.file_c_str(), new_path.file_c_str() ) )
#   endif
        throw filesystem_error( std::string("move_file(): ")
          + old_path.file_c_str() + ", " + new_path.file_c_str(), system_error );
    }

    void copy_file( const path & from_file_path,
                    const path & to_file_path )
    {
#   ifdef BOOST_POSIX
      // TODO: Ask POSIX experts if this is the best way to copy a file

      const std::size_t buf_sz = 32768;
      boost::scoped_array<char> buf( new char [buf_sz] );
      int infile, outfile;

      if ( (infile = ::open( from_file_path.file_c_str(),
                             O_RDONLY )) < 0
        || (outfile = ::open( to_file_path.file_c_str(),
                              O_WRONLY | O_CREAT | O_EXCL,
                              S_IRWXU|S_IRWXG|S_IRWXO )) < 0 )
      {
        if ( infile != 0 ) ::close( infile );
        throw fs::filesystem_error( std::string("copy() files: ")
          + from_file_path.file_c_str()
          + ", " + to_file_path.file_c_str(), system_error );
      }

      ssize_t sz;
      while ( (sz = ::read( infile, buf.get(), buf_sz )) > 0
        && (sz = ::write( outfile, buf.get(), sz )) > 0 ) {}

      ::close( infile );
      ::close( outfile );

      if ( sz != 0 )
#   else
      if ( !CopyFile( from_file_path.file_c_str(),
                      to_file_path.file_c_str(), /*fail_if_exists=*/true ) )
#   endif
        throw fs::filesystem_error( std::string("copy() files: ")
          + from_file_path.file_c_str()
          + ", " + to_file_path.file_c_str(), system_error );
    }

    const path & initial_directory()
    {
      static path init_dir;
      if ( init_dir.empty() )
      {
#     ifdef BOOST_POSIX
        long path_max = ::pathconf( ".", _PC_PATH_MAX );
        if ( path_max == -1 )
          throw filesystem_error( "initial_directory()" );
        boost::scoped_array<char>
          buf( new char[static_cast<std::size_t>(path_max)] );
        if ( ::getcwd( buf.get(), static_cast<std::size_t>(path_max) ) == 0 )
#     else
        DWORD sz;
        if ( (sz = GetCurrentDirectory( 0, static_cast<char*>(0) )) == 0 )
          throw filesystem_error( "initial_directory()" );
        boost::scoped_array<char> buf( new char[sz] );
        if ( GetCurrentDirectory( sz, buf.get() ) == 0 )
#     endif
          { throw filesystem_error( "initial_directory()", system_error ); }
        init_dir = path( buf.get(), path::system_specific );
      }
      return init_dir;
    }

  } // namespace filesystem
} // namespace boost

