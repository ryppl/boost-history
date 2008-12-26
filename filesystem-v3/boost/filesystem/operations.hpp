//  boost/filesystem/operations.hpp  ---------------------------------------------------//

//  Copyright Beman Dawes 2002-2008
//  Copyright Jan Langer 2002
//  Copyright Dietmar Kuehl 2001                                        
//  Copyright Vladimir Prus 2002
   
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/filesystem

//--------------------------------------------------------------------------------------//
/*
*                                TODO List
*
*  Scoped enum workaround for file_type.
*  Enable all BOOST_FILESYSTEM_NO_DEPRECATED code.
*  Vista symlink_status support.
*  Merge fixes from trunk
*  rename and remove really need to be renamed. If users says "using namespace boost::filesystem"
   and some header included stdio, there is just too much chance of silent error.
*  create_directories error handling.
*  Review any commented out code, both in operations.hpp and operations.cpp
*  Finish refactoring operations_test.
*  Fold convenience.hpp into operations.hpp
*  Two argument recursive_directory_iterator ctor isn't recognizing system::throws.
   would it be better to fold into a single two argument ctor with default?
*  Add the push_directory class from tools/release/required_files.cpp

*/
//--------------------------------------------------------------------------------------// 

#ifndef BOOST_FILESYSTEM_OPERATIONS_HPP
#define BOOST_FILESYSTEM_OPERATIONS_HPP

#include <boost/filesystem/path.hpp>

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/iterator.hpp>
#include <boost/cstdint.hpp>
#include <boost/assert.hpp>

#include <string>
#include <utility> // for pair
#include <ctime>

#ifdef BOOST_WINDOWS_API
#  include <fstream>
#  if !defined(_WIN32_WINNT)
#    define _WIN32_WINNT 0x0500	// assume Windows 2000 or later SDK
#  endif
#endif

#include <boost/config/abi_prefix.hpp> // must be the last #include

# ifdef BOOST_NO_STDC_NAMESPACE
    namespace std { using ::time_t; }
# endif

//--------------------------------------------------------------------------------------//

namespace boost
{
  namespace filesystem
  {

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                            support classes and enums                                 //
//                                                                                      //
//--------------------------------------------------------------------------------------//

  enum file_type
  { 
    status_unknown,
    file_not_found,
    regular_file,
    directory_file,
    // the following will never be reported by some operating or file systems
    symlink_file,
    block_file,
    character_file,
    fifo_file,
    socket_file,
    type_unknown // file does exist, but isn't one of the above types or
                 // we don't have strong enough permission to find its type
  };

  class BOOST_FILESYSTEM_DECL file_status
  {
  public:
    explicit file_status( file_type v = status_unknown ) : m_value(v) {}

    void type( file_type v )  { m_value = v; }
    file_type type() const    { return m_value; }

    bool operator==( const file_status & rhs ) const { return type() == rhs.type(); }
    bool operator!=( const file_status & rhs ) const { return !(*this == rhs); }

  private:
    // the internal representation is unspecified so that additional state
    // information such as permissions can be added in the future; this
    // implementation just uses status_type as the internal representation

    file_type m_value;
  };

  inline bool status_known( file_status f ) { return f.type() != status_unknown; }
  inline bool exists( file_status f )       { return f.type() != status_unknown && f.type() != file_not_found; }
  inline bool is_regular_file(file_status f){ return f.type() == regular_file; }
  inline bool is_directory( file_status f ) { return f.type() == directory_file; }
  inline bool is_symlink( file_status f )   { return f.type() == symlink_file; }
  inline bool is_other( file_status f )     { return exists(f) && !is_regular_file(f) && !is_directory(f) && !is_symlink(f); }

# ifndef BOOST_FILESYSTEM_NO_DEPRECATED
  inline bool is_regular( file_status f )   { return f.type() == regular_file; }
# endif

  struct space_info
  {
    // all values are byte counts
    boost::uintmax_t capacity;
    boost::uintmax_t free;      // <= capacity
    boost::uintmax_t available; // <= free
  };

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                                 query functions                                      //
//                                                                                      //
//--------------------------------------------------------------------------------------//

  BOOST_FILESYSTEM_DECL
  file_status status( const path & p,
                      system::error_code & ec = system::throws );

  BOOST_FILESYSTEM_DECL
  file_status symlink_status( const path & p,
                      system::error_code & ec = system::throws );
    
  inline bool exists( const path & p, system::error_code & ec = system::throws ) { return exists( status( p, ec ) ); }
  inline bool is_directory( const path & p, system::error_code & ec = system::throws )  { return is_directory( status( p, ec ) ); }
  inline bool is_regular_file( const path & p, system::error_code & ec = system::throws )  { return is_regular_file( status( p, ec ) ); }
  inline bool is_other( const path & p, system::error_code & ec = system::throws ) { return is_other( status( p, ec ) ); }
  inline bool is_symlink( const path & p, system::error_code & ec = system::throws ) { return is_symlink( symlink_status( p, ec ) ); }

#   ifndef BOOST_FILESYSTEM_NO_DEPRECATED
  inline bool is_regular( const path & p, system::error_code & ec = system::throws ) { return is_regular( status( p, ec ) ); }
#   endif


//    // VC++ 7.0 and earlier has a serious namespace bug that causes a clash
//    // between boost::filesystem::is_empty and the unrelated type trait
//    // boost::is_empty.
//
//# if !defined( BOOST_MSVC ) || BOOST_MSVC > 1300
//    BOOST_FS_FUNC(bool) is_empty( const Path & ph )
//# else
//    BOOST_FS_FUNC(bool) _is_empty( const Path & ph )
//# endif
//    {
//      detail::query_pair result(
//        detail::is_empty_api( ph.external_file_string() ) );
//      if ( result.first )
//        boost::throw_exception( basic_filesystem_error<Path>(
//          "boost::filesystem::is_empty", ph, result.first ) );
//      return result.second;
//    }

  BOOST_FILESYSTEM_DECL
  bool is_empty( const path & p, system::error_code & ec = system::throws );

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                             operational functions                                    //
//                  in alphabetical order, unless otherwise noted                       //
//                                                                                      //
//--------------------------------------------------------------------------------------//

  BOOST_FILESYSTEM_DECL // declaration must precede complete()
  path initial_path( system::error_code & ec = system::throws );

  BOOST_FILESYSTEM_DECL
  path complete( const path & p, const path & base = initial_path() );

  BOOST_FILESYSTEM_DECL
  void copy_file( const path & from, const path & to,
                  system::error_code & ec = system::throws );

  BOOST_FILESYSTEM_DECL
  bool create_directories( const path & p/*,
                  system::error_code & ec = system::throws*/ );

  BOOST_FILESYSTEM_DECL
  bool create_directory( const path & p, system::error_code & ec = system::throws );

  BOOST_FILESYSTEM_DECL
  void create_directory_symlink( const path & to, const path & from,
                                 system::error_code & ec = system::throws );

  BOOST_FILESYSTEM_DECL
  void create_hard_link( const path & to, const path & from,
                         system::error_code & ec = system::throws );

  BOOST_FILESYSTEM_DECL
  void create_symlink( const path & to, const path & from,
                       system::error_code & ec = system::throws );

  BOOST_FILESYSTEM_DECL
  path current_path( system::error_code & ec = system::throws );

  BOOST_FILESYSTEM_DECL
  void current_path( const path & p, system::error_code & ec = system::throws );

  BOOST_FILESYSTEM_DECL
  bool equivalent( const path & p1, const path & p2,
                   system::error_code & ec = system::throws );

  BOOST_FILESYSTEM_DECL
  boost::uintmax_t file_size( const path & p,
                   system::error_code & ec = system::throws );

  //  initial_path() declaration precedes complete()

  BOOST_FILESYSTEM_DECL
  std::time_t last_write_time( const path & p,
                   system::error_code & ec = system::throws );

  BOOST_FILESYSTEM_DECL
  void last_write_time( const path & p, const std::time_t new_time,
                   system::error_code & ec = system::throws );

  BOOST_FILESYSTEM_DECL
  bool remove( const path & p, system::error_code & ec = system::throws );

  BOOST_FILESYSTEM_DECL
  boost::uintmax_t remove_all( const path & p,
                   system::error_code & ec = system::throws );

  BOOST_FILESYSTEM_DECL
  void rename( const path & from, const path & to,
                   system::error_code & ec = system::throws );

  BOOST_FILESYSTEM_DECL
  space_info space( const path & p, system::error_code & ec = system::throws ); 

  BOOST_FILESYSTEM_DECL
  path system_complete( const path & p, system::error_code & ec = system::throws ); 

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                                 directory_entry                                      //
//                                                                                      //
//--------------------------------------------------------------------------------------//

//  GCC has a problem with a member function named path within a namespace or 
//  sub-namespace that also has a class named path. The workaround is to always
//  fully qualify the name path when it refers to the class name.

class BOOST_FILESYSTEM_DECL directory_entry
{
public:

  // compiler generated copy-ctor, copy assignment, and destructor apply

  directory_entry() {}
  explicit directory_entry( const boost::filesystem::path & p,
    file_status st = file_status(), file_status symlink_st=file_status() )
    : m_path(p), m_status(st), m_symlink_status(symlink_st)
    {}

  void assign( const boost::filesystem::path & p,
    file_status st, file_status symlink_st )
    { m_path = p; m_status = st; m_symlink_status = symlink_st; }

  void replace_filename( const boost::filesystem::path & p,
    file_status st, file_status symlink_st )
  {
    m_path.remove_filename();
    m_path /= p;
    m_status = st;
    m_symlink_status = symlink_st;
  }

# ifndef BOOST_FILESYSTEM_NO_DEPRECATED
  void replace_leaf( const boost::filesystem::path & p,
    file_status st, file_status symlink_st )
      { replace_filename( p, st, symlink_st ); }
# endif

  const boost::filesystem::path &  path() const { return m_path; }
  file_status   status( system::error_code & ec = system::throws ) const;
  file_status   symlink_status( system::error_code & ec = system::throws ) const;

  // conversion simplifies the most common use of directory_entry
  operator const boost::filesystem::path &() const { return m_path; }

//#   ifndef BOOST_FILESYSTEM_NO_DEPRECATED
//      // deprecated functions preserve common use cases in legacy code
//      typename Path::string_type filename() const
//      {
//        return path().filename();
//      }
//      typename Path::string_type leaf() const
//      {
//        return path().filename();
//      }
//      typename Path::string_type string() const
//      {
//        return path().string();
//      }
//#   endif

private:
  boost::filesystem::path   m_path;
  mutable file_status       m_status;           // stat()-like
  mutable file_status       m_symlink_status;   // lstat()-like

}; // directory_entry

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                            directory_iterator helpers                                //
//                                                                                      //
//--------------------------------------------------------------------------------------//

class directory_iterator;

namespace detail
{
  BOOST_FILESYSTEM_DECL
    system::error_code dir_itr_close(  // never throws
    void *& handle
#     if     defined(BOOST_POSIX_API)
        , void *& buffer
#     endif
    ); 

  struct BOOST_FILESYSTEM_DECL dir_itr_imp
  {
    directory_entry  dir_entry;
    void *           handle;

#   ifdef BOOST_POSIX_API
      void *           buffer;  // see dir_itr_increment implementation
#   endif

    dir_itr_imp() : handle(0)
#     ifdef BOOST_POSIX_API
        , buffer(0)
#     endif
    {}

    ~dir_itr_imp() // never throws
    {
      dir_itr_close( handle
#       if defined(BOOST_POSIX_API)
         , buffer
#       endif
      );
    }
  };

  // see path::iterator: comment below
  BOOST_FILESYSTEM_DECL void directory_iterator_construct( directory_iterator & it,
    const path & p, system::error_code & ec );
  BOOST_FILESYSTEM_DECL void directory_iterator_increment( directory_iterator & it );

}  // namespace detail

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                                directory_iterator                                    //
//                                                                                      //
//--------------------------------------------------------------------------------------//

  class directory_iterator
    : public boost::iterator_facade<
        directory_iterator,
        directory_entry,
        boost::single_pass_traversal_tag >
  {
  public:

    directory_iterator(){}  // creates the "end" iterator

    // iterator_facade derived classes don't seem to like implementations in
    // separate translation unit dll's, so forward to detail functions
    directory_iterator( const path & p,
      system::error_code & ec = system::throws )
        : m_imp( new detail::dir_itr_imp )
          { detail::directory_iterator_construct( *this, p, ec ); }

    ~directory_iterator() {} // never throws

  private:
    friend struct detail::dir_itr_imp;
    friend void detail::directory_iterator_construct( directory_iterator & it,
      const path & p, system::error_code & ec);
    friend void detail::directory_iterator_increment( directory_iterator & it );

    // shared_ptr provides shallow-copy semantics required for InputIterators.
    // m_imp.get()==0 indicates the end iterator.
    boost::shared_ptr< detail::dir_itr_imp >  m_imp;

    friend class boost::iterator_core_access;

    boost::iterator_facade<
      directory_iterator,
      directory_entry,
      boost::single_pass_traversal_tag >::reference dereference() const 
    {
      BOOST_ASSERT( m_imp.get() && "attempt to dereference end iterator" );
      return m_imp->dir_entry;
    }

    void increment() { detail::directory_iterator_increment( *this ); }

    bool equal( const directory_iterator & rhs ) const
      { return m_imp == rhs.m_imp; }
  };

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                            class filesystem_error                                    //
//                                                                                      //
//--------------------------------------------------------------------------------------//
  
  class filesystem_error : public system::system_error
  {
  // see http://www.boost.org/more/error_handling.html for design rationale

  // all functions are inline to avoid issues with crossing dll boundaries

  public:
    // compiler generates copy constructor and copy assignment

    // constructors without path arguments

    filesystem_error::filesystem_error(
      const std::string & what_arg, system::error_code ec )
      : system::system_error(ec, what_arg)
    {
      try
      {
        m_imp_ptr.reset( new m_imp );
      }
      catch (...) { m_imp_ptr.reset(); }
    }

    filesystem_error::filesystem_error(
      const char * what_arg, system::error_code ec )
      : system::system_error(ec, what_arg)
    {
      try
      {
        m_imp_ptr.reset( new m_imp );
      }
      catch (...) { m_imp_ptr.reset(); }
    }

    filesystem_error::filesystem_error(
      const std::string & what_arg, int ev, const system::error_category & ecat )
      : system::system_error(ev, ecat, what_arg)
    {
      try
      {
        m_imp_ptr.reset( new m_imp );
      }
      catch (...) { m_imp_ptr.reset(); }
    }

    filesystem_error::filesystem_error(
      const char * what_arg, int ev, const system::error_category & ecat )
      : system::system_error(ev, ecat, what_arg)
    {
      try
      {
        m_imp_ptr.reset( new m_imp );
      }
      catch (...) { m_imp_ptr.reset(); }
    }

    // constructors with one path argument

    filesystem_error::filesystem_error(
      const std::string & what_arg, const path & path1_arg,
      system::error_code ec )
      : system::system_error(ec, what_arg)
    {
      try
      {
        m_imp_ptr.reset( new m_imp );
        m_imp_ptr->m_path1 = path1_arg;
      }
      catch (...) { m_imp_ptr.reset(); }
    }

    filesystem_error::filesystem_error(
      const char * what_arg, const path & path1_arg,
      system::error_code ec )
      : system::system_error(ec, what_arg)
    {
      try
      {
        m_imp_ptr.reset( new m_imp );
        m_imp_ptr->m_path1 = path1_arg;
      }
      catch (...) { m_imp_ptr.reset(); }
    }

    filesystem_error::filesystem_error(
      const std::string & what_arg, const path & path1_arg,
      int ev, const system::error_category & ecat )
      : system::system_error(ev, ecat, what_arg)
    {
      try
      {
        m_imp_ptr.reset( new m_imp );
        m_imp_ptr->m_path1 = path1_arg;
      }
      catch (...) { m_imp_ptr.reset(); }
    }

    filesystem_error::filesystem_error(
      const char * what_arg, const path & path1_arg,
      int ev, const system::error_category & ecat )
      : system::system_error(ev, ecat, what_arg)
    {
      try
      {
        m_imp_ptr.reset( new m_imp );
        m_imp_ptr->m_path1 = path1_arg;
      }
      catch (...) { m_imp_ptr.reset(); }
    }

    // constructors with two path arguments
    
    filesystem_error::filesystem_error(
      const std::string & what_arg, const path & path1_arg,
      const path & path2_arg, system::error_code ec )
      : system::system_error(ec, what_arg)
    {
      try
      {
        m_imp_ptr.reset( new m_imp );
        m_imp_ptr->m_path1 = path1_arg;
        m_imp_ptr->m_path2 = path2_arg;
      }
      catch (...) { m_imp_ptr.reset(); }
    }

    filesystem_error::filesystem_error(
      const char * what_arg, const path & path1_arg,
      const path & path2_arg, system::error_code ec )
      : system::system_error(ec, what_arg)
    {
      try
      {
        m_imp_ptr.reset( new m_imp );
        m_imp_ptr->m_path1 = path1_arg;
        m_imp_ptr->m_path2 = path2_arg;
      }
      catch (...) { m_imp_ptr.reset(); }
    }

    filesystem_error::filesystem_error(
      const std::string & what_arg, const path & path1_arg,
      const path & path2_arg, int ev, const system::error_category & ecat )
      : system::system_error(ev, ecat, what_arg)
    {
      try
      {
        m_imp_ptr.reset( new m_imp );
        m_imp_ptr->m_path1 = path1_arg;
        m_imp_ptr->m_path2 = path2_arg;
      }
      catch (...) { m_imp_ptr.reset(); }
    }

    filesystem_error::filesystem_error(
      const char * what_arg, const path & path1_arg,
      const path & path2_arg, int ev, const system::error_category & ecat )
      : system::system_error(ev, ecat, what_arg)
    {
      try
      {
        m_imp_ptr.reset( new m_imp );
        m_imp_ptr->m_path1 = path1_arg;
        m_imp_ptr->m_path2 = path2_arg;
      }
      catch (...) { m_imp_ptr.reset(); }
    }

    ~filesystem_error() throw() {}

    const path & path1() const
    {
      static const path empty_path;
      return m_imp_ptr.get() ? m_imp_ptr->m_path1 : empty_path ;
    }
    const path & path2() const
    {
      static const path empty_path;
      return m_imp_ptr.get() ? m_imp_ptr->m_path2 : empty_path ;
    }

    const char * filesystem_error::what() const throw()
    {
      if ( !m_imp_ptr.get() )
        return system::system_error::what();

      try
      {
        if ( m_imp_ptr->m_what.empty() )
        {
          m_imp_ptr->m_what = system::system_error::what();
          if ( !m_imp_ptr->m_path1.empty() )
          {
            m_imp_ptr->m_what += ": \"";
            m_imp_ptr->m_what += m_imp_ptr->m_path1.string();
            m_imp_ptr->m_what += "\"";
          }
          if ( !m_imp_ptr->m_path2.empty() )
          {
            m_imp_ptr->m_what += ", \"";
            m_imp_ptr->m_what += m_imp_ptr->m_path2.string();
            m_imp_ptr->m_what += "\"";
          }
        }
        return m_imp_ptr->m_what.c_str();
      }
      catch (...)
      {
        return system::system_error::what();
      }
    }

  private:
    struct m_imp
    {
      path         m_path1; // may be empty()
      path         m_path2; // may be empty()
      std::string  m_what;  // not built until needed
    };
    boost::shared_ptr<m_imp> m_imp_ptr;
  };

//  test helper  -----------------------------------------------------------------------//

//  Not part of the documented interface since false positives are possible;
//  there is no law that says that an OS that has large stat.st_size
//  actually supports large file sizes.

  namespace detail
  {
    BOOST_FILESYSTEM_DECL bool possible_large_file_size_support();
  }

  } // namespace filesystem
} // namespace boost


#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas
#endif // BOOST_FILESYSTEM_OPERATIONS_HPP
