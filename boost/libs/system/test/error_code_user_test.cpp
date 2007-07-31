//  error_code_user_test.cpp  ------------------------------------------------//

//  Copyright Beman Dawes 2006

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/system

//  ------------------------------------------------------------------------  //

//  This program demonstrates creation and use of new categories of error
//  codes. Several scenarios are demonstrated and tested.

//  Motivation was a Boost posting by Christopher Kohlhoff on June 28, 2006.

#include <boost/system/error_code.hpp>
#include <boost/cerrno.hpp>
#include <string>
#include <cstdio>
#include <boost/test/minimal.hpp>

#ifdef BOOST_POSIX_API
# include <sys/stat.h>
#else
# include <windows.h>
#endif

//  ------------------------------------------------------------------------  //

//  Library 1: User function passes through an error code from the
//  operating system. 


boost::system::error_code my_mkdir( const std::string & path )
{
  return boost::system::error_code(
#   ifdef BOOST_POSIX_API
      ::mkdir( path.c_str(), S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH ) == 0 ? 0 : errno,
#   else
      ::CreateDirectoryA( path.c_str(), 0 ) != 0 ? 0 : ::GetLastError(),
#   endif
      boost::system::system_category );
}

//  ------------------------------------------------------------------------  //

//  Library 2: User function passes through errno from the C-runtime. 

#include <cstdio>

boost::system::error_code my_remove( const std::string & path )
{
  return boost::system::error_code(
    ::remove( path.c_str() ) == 0 ? 0 : errno,
    boost::system::posix_category ); // OK for both Windows and POSIX
                                     // Alternatively, could use posix_category
                                     // on Windows and system_category on
                                     // POSIX-based systems.
}

//  ------------------------------------------------------------------------  //

//  Library 3: Library uses enum to identify library specific errors.

//  This particular example is for a library within the parent namespace. For
//  an example of a library not within the parent namespace, see library 4.

//  header lib3.hpp:

namespace boost
{
  namespace lib3
  {
    // lib3 has its own error_category:
    extern const boost::system::error_category & lib3_error_category;
    
    enum error
    {
      boo_boo=123,
      big_boo_boo
    };

    inline boost::system::error_code make_error_code(error e)
      { return boost::system::error_code(e,lib3_error_category); }
  }
}

//  implementation file lib3.cpp:

//  #include <lib3.hpp>

namespace boost
{
  namespace lib3
  {
    class lib3_error_category_imp : public boost::system::error_category
    {
    public:
      const std::string & name() const
      {
        static std::string s( "lib3" );
        return s;
      }

      boost::system::posix::posix_errno  posix( int ev ) const
      {
        return ev == boo_boo
          ? boost::system::posix::io_error
          : boost::system::posix::no_posix_equivalent;
      }
      
      std::string message( int ev ) const
      {
        if ( ev == boo_boo ) return std::string("boo boo");
        if ( ev == big_boo_boo ) return std::string("big boo boo");
        return std::string("unknown error");
      }
    };

    const lib3_error_category_imp lib3_error_category_const;

    const boost::system::error_category & lib3_error_category
      = lib3_error_category_const;
  }
}

//  ------------------------------------------------------------------------  //

//  Library 4: Library uses const error_code's to identify library specific
//  errors. 

//  This particular example is for a library not within the parent namespace.
//  For an example of a library within the parent namespace, see library 3.

//  header lib4.hpp:

namespace lib4
{
  // lib4 has its own error_category:
  extern const boost::system::error_category & lib4_error_category;
  
  extern const boost::system::error_code boo_boo;
  extern const boost::system::error_code big_boo_boo;
}

//  implementation file lib4.cpp:

//  #include <lib4.hpp>

namespace lib4
{
  class lib4_error_category_imp : public boost::system::error_category
  {
  public:
    const std::string & name() const
    {
      static std::string s( "lib4" );
      return s;
    }

    boost::system::posix::posix_errno  posix( int ev ) const
    {
      return ev == boo_boo.value()
        ? boost::system::posix::io_error
        : boost::system::posix::no_posix_equivalent;
    }
    
    std::string message( int ev ) const
    {
      if ( ev == boo_boo.value() ) return std::string("boo boo");
      if ( ev == big_boo_boo.value() ) return std::string("big boo boo");
      return std::string("unknown error");
    }
  };

  const lib4_error_category_imp lib4_error_category_const;

  const boost::system::error_category & lib4_error_category
    = lib4_error_category_const;

  const boost::system::error_code boo_boo( 456, lib4_error_category );
  const boost::system::error_code big_boo_boo( 789, lib4_error_category );

}

//  ------------------------------------------------------------------------  //

int test_main( int, char *[] )
{
  boost::system::error_code ec;

  // Library 1 tests:
  
  ec = my_mkdir( "/no-such-file-or-directory/will-not-succeed" );
  std::cout << "ec.value() is " << ec.value() << '\n';

  BOOST_CHECK( ec );
  BOOST_CHECK( ec == boost::system::posix::no_such_file_or_directory );
  BOOST_CHECK( ec.category() == boost::system::system_category );

  // Library 2 tests:

  ec = my_remove( "/no-such-file-or-directory" );
  std::cout << "ec.value() is " << ec.value() << '\n';

  BOOST_CHECK( ec );
  BOOST_CHECK( ec == boost::system::posix::no_such_file_or_directory );
  BOOST_CHECK( ec.category() == boost::system::posix_category );

  // Library 3 tests:

  ec = boost::lib3::boo_boo;
  std::cout << "ec.value() is " << ec.value() << '\n';

  BOOST_CHECK( ec );
  BOOST_CHECK( ec == boost::lib3::boo_boo );
  BOOST_CHECK( ec.value() == boost::lib3::boo_boo );
  BOOST_CHECK( ec.category() == boost::lib3::lib3_error_category );

  BOOST_CHECK( ec.posix() == boost::system::posix::io_error );
  BOOST_CHECK( ec == boost::system::posix::io_error );

  boost::system::error_code ec3( boost::lib3::boo_boo+100,
    boost::lib3::lib3_error_category );
  BOOST_CHECK( ec3.posix() == boost::system::posix::no_posix_equivalent );

  // Library 4 tests:

  ec = lib4::boo_boo;
  std::cout << "ec.value() is " << ec.value() << '\n';

  BOOST_CHECK( ec );
  BOOST_CHECK( ec == lib4::boo_boo );
  BOOST_CHECK( ec.value() == lib4::boo_boo.value() );
  BOOST_CHECK( ec.category() == lib4::lib4_error_category );

  BOOST_CHECK( ec.posix() == boost::system::posix::io_error );
  BOOST_CHECK( ec == boost::system::posix::io_error );

  boost::system::error_code ec4( lib4::boo_boo.value()+100,
    lib4::lib4_error_category );
  BOOST_CHECK( ec4.posix() == boost::system::posix::no_posix_equivalent );


  return 0;
}
