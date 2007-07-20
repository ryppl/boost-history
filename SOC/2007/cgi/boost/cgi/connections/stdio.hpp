//                     -- stdio.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_STDIO_CONNECTION_IMPL_HPP_INCLUDED__
#define CGI_STDIO_CONNECTION_IMPL_HPP_INCLUDED__

#include <iostream>
#include <istream>
#include <ostream>
#include <boost/system/error_code.hpp>

#include "../basic_connection_fwd.hpp"
#include "../tags.hpp"
#include "../connection_base.hpp"
//#include "../io_service.hpp"

namespace cgi {

  template<>
  class basic_connection<tags::stdio>
    : public connection_base
  {
  public:
    basic_connection()
      : in_(std::cin)
      , out_(std::cout)
      , err_(std::cerr)
    {
    }

    template<typename T>
    basic_connection(T&)
      : in_(std::cin)
      , out_(std::cout)
      , err_(std::cerr)
    {
    }

    template<typename MutableBufferSequence>
    std::size_t read_some(MutableBufferSequence buf
                         , boost::system::error_code& ec)
    {
      if( buf.data() != in_.rdbuf() )
        return in_.read(buf.data(), buf.size());
      return buf.size();
    }

    template<typename ConstBufferSequence>
    std::size_t write_some(ConstBufferSequence& buf
                          , boost::system::error_code& ec)
    {
      return out_.write(buf.data(), buf.size());
    }

  protected:
    std::istream& in_;
    std::ostream& out_;
    std::ostream& err_;
  };

  typedef basic_connection<tags::stdio> stdio_connection;

  //  template<typename ProtocolService = detail::cgi_service>
  //struct stdio_connection
  //{
  //  typedef basic_connection<tags::stdio, ProtocolService>    type;
  //};

} // namespace cgi

#endif // CGI_STDIO_CONNECTION_IMPL_HPP_INCLUDED__
