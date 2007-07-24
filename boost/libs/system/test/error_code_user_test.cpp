//  error_code_user_test.cpp  ------------------------------------------------//

//  Copyright Beman Dawes 2006

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/system

//  ------------------------------------------------------------------------  //
//  This code demonstrates creation and use of a new category of error codes.

//  The motivation was a Boost posting by Christopher Kohlhoff on June 28, 2006.

#include <boost/system/error_code.hpp>
#include <boost/cerrno.hpp>


//  ------------------------------------------------------------------------  //

//  header asio.hpp

#define BOO_BOO 12345  // this could also be a constant; a macro is used for
                       // illustration because many older API's define errors
                       // via macro.
namespace boost
{
  namespace asio
  {
    // asio has its own error_category:
    extern const boost::system::error_category & asio_error_category;
    
    enum error
    {
      boo_boo = BOO_BOO
    };

    inline boost::system::error_code make_error_code(error e)
      { return boost::system::error_code(e,asio_error_category); }
  }
}

//  ------------------------------------------------------------------------  //

//  implementation file asio.cpp:

//  #include <asio.hpp>

namespace boost
{
  namespace asio
  {
    class asio_error_category_imp : public boost::system::error_category
    {
    public:
      const std::string & name() const
      {
        static std::string s( "asio" );
        return s;
      }

      boost::system::posix_errno  posix( int ev ) const
      {
        return ev == BOO_BOO
          ? boost::system::io_error
          : boost::system::no_posix_equivalent;
      }
    };

    const asio_error_category_imp asio_error_category_const;
    const boost::system::error_category & asio_error_category
      = asio_error_category_const;
  }
}

//  ------------------------------------------------------------------------  //

//  a user program:


// #include <asio.hpp>
#include <boost/test/minimal.hpp>

int test_main( int, char *[] )
{
  boost::system::error_code ec( boost::asio::boo_boo );

  BOOST_CHECK( ec );
  BOOST_CHECK( ec == boost::asio::boo_boo );
  BOOST_CHECK( ec.value() == BOO_BOO );
  BOOST_CHECK( ec.value() == boost::asio::boo_boo );
  BOOST_CHECK( ec.category() == boost::asio::asio_error_category );

  BOOST_CHECK( ec.posix() == boost::system::io_error );

  boost::system::error_code ec2( boost::asio::boo_boo+1,
    boost::asio::asio_error_category );
  BOOST_CHECK( ec2.posix() == boost::system::no_posix_equivalent );
  return 0;
}
