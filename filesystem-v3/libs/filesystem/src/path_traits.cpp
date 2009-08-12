//  filesystem convert.cpp  ------------------------------------------------------------//

//  Copyright Beman Dawes 2008, 2009

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  Library home page: http://www.boost.org/libs/filesystem

#include <boost/filesystem/detail/convert.hpp>
#include <boost/filesystem/config.hpp>
#include <boost/scoped_array.hpp>
#include <cstring>  // for strlen
#include <cwchar>   // for wcslen

//--------------------------------------------------------------------------------------//
//                                  configuration                                       //
//--------------------------------------------------------------------------------------//

#ifndef BOOST_FILESYSTEM_CODECVT_BUF_SIZE
# define BOOST_FILESYSTEM_CODECVT_BUF_SIZE 256
#endif

namespace {

  const std::size_t default_codecvt_buf_size = BOOST_FILESYSTEM_CODECVT_BUF_SIZE;


//--------------------------------------------------------------------------------------//
//                                                                                      //
//  The public convert() functions do buffer management, and then forward to the        //
//  convert_aux() functions for the actual call to the codecvt facet.                   //
//                                                                                      //
//--------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------//
//                      convert_aux const char * to wstring                             //
//--------------------------------------------------------------------------------------//

  void convert_aux(
                   const char * from,
                   const char * from_end,
                   wchar_t * to, wchar_t * to_end,
                   std::wstring & target,
                   const boost::filesystem::codecvt_type & cvt )
  {
    //std::cout << std::hex
    //          << " from=" << std::size_t(from)
    //          << " from_end=" << std::size_t(from_end)
    //          << " to=" << std::size_t(to)
    //          << " to_end=" << std::size_t(to_end)
    //          << std::endl;

    std::mbstate_t state  = std::mbstate_t();  // perhaps unneeded, but cuts bug reports
    const char * from_next;
    wchar_t * to_next;

    std::codecvt_base::result res;

    if ( (res=cvt.in( state, from, from_end, from_next,
           to, to_end, to_next )) != std::codecvt_base::ok )
    {
      //std::cout << " result is " << static_cast<int>(res) << std::endl;
      assert( 0 && "append error handling not implemented yet" );
      throw "append error handling not implemented yet";
    }
    target.append( to, to_next ); 
  }

//--------------------------------------------------------------------------------------//
//                      convert_aux const wchar_t * to string                           //
//--------------------------------------------------------------------------------------//

  void convert_aux(
                   const wchar_t * from,
                   const wchar_t * from_end,
                   char * to, char * to_end,
                   std::string & target,
                   const boost::filesystem::codecvt_type & cvt )
  {
    //std::cout << std::hex
    //          << " from=" << std::size_t(from)
    //          << " from_end=" << std::size_t(from_end)
    //          << " to=" << std::size_t(to)
    //          << " to_end=" << std::size_t(to_end)
    //          << std::endl;

    std::mbstate_t state  = std::mbstate_t();  // perhaps unneeded, but cuts bug reports
    const wchar_t * from_next;
    char * to_next;

    std::codecvt_base::result res;

    if ( (res=cvt.out( state, from, from_end, from_next,
           to, to_end, to_next )) != std::codecvt_base::ok )
    {
      //std::cout << " result is " << static_cast<int>(res) << std::endl;
      assert( 0 && "append error handling not implemented yet" );
      throw "append error handling not implemented yet";
    }
    target.append( to, to_next ); 
  }
  
}  // unnamed namespace

namespace boost { namespace filesystem { namespace detail {

//--------------------------------------------------------------------------------------//
//                          convert const char * to wstring                             //
//--------------------------------------------------------------------------------------//

  BOOST_FILESYSTEM_DECL
  void convert( const char * from,
                const char * from_end,    // 0 for null terminated MBCS
                std::wstring & to,
                const codecvt_type & cvt )
  {
    BOOST_ASSERT( from );

    if ( !from_end )  // null terminated
    {
      from_end = from + std::strlen( from );
    }

    if ( from == from_end ) return;

    std::size_t buf_size = (from_end - from) * 3;  // perhaps too large, but that's OK

    //  dynamically allocate a buffer only if source is unusually large
    if ( buf_size > default_codecvt_buf_size )
    {
      boost::scoped_array< wchar_t > buf( new wchar_t [buf_size] );
      convert_aux( from, from_end, buf.get(), buf.get()+buf_size, to, cvt );
    }
    else
    {
      wchar_t buf[default_codecvt_buf_size];
      convert_aux( from, from_end, buf, buf+default_codecvt_buf_size, to, cvt );
    }
  }

//--------------------------------------------------------------------------------------//
//                         convert const wchar_t * to string                            //
//--------------------------------------------------------------------------------------//

  BOOST_FILESYSTEM_DECL
  void convert( const wchar_t * from,
                const wchar_t * from_end,  // 0 for null terminated MBCS
                std::string & to,
                const codecvt_type & cvt )
  {
    BOOST_ASSERT( from );

    if ( !from_end )  // null terminated
    {
      from_end = from + std::wcslen( from );
    }

    if ( from == from_end ) return;

    //  The codecvt length functions may not be implemented, and I don't really
    //  understand them either. Thus this code is just a guess; if it turns
    //  out the buffer is too small then an error will be reported and the code
    //  will have to be fixed.
    std::size_t buf_size = (from_end - from) * 4;  // perhaps too large, but that's OK
    buf_size += 4;  // encodings like shift-JIS need some prefix space

    //  dynamically allocate a buffer only if source is unusually large
    if ( buf_size > default_codecvt_buf_size )
    {
      boost::scoped_array< char > buf( new char [buf_size] );
      convert_aux( from, from_end, buf.get(), buf.get()+buf_size, to, cvt );
    }
    else
    {
      char buf[default_codecvt_buf_size];
      convert_aux( from, from_end, buf, buf+default_codecvt_buf_size, to, cvt );
    }
  }
}}} // namespace boost::filesystem::detail
