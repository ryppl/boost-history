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
#include <string>
#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>

#include "boost/cgi/basic_connection_fwd.hpp"
#include "boost/cgi/tags.hpp"
#include "boost/cgi/connection_base.hpp"
#include "boost/cgi/data_sink.hpp"
//#include "boost/cgi/io_service.hpp"

namespace cgi {

  template<>
  class basic_connection<tags::stdio>
    : public connection_base
  {
  public:
    typedef boost::shared_ptr<basic_connection<tags::stdio> >  pointer;

    basic_connection()
      : is_open_(true)
    {
    }

    template<typename T>
    basic_connection(T&)
      : is_open_(true)
    {
    }

    bool is_open() const
    {
      return is_open_;
    }

    void close()
    {
      is_open_ = false;
    }

    static pointer create()
    {
      return pointer(new basic_connection<tags::stdio>());
    }

    template<typename MutableBufferSequence>
    std::size_t read_some(MutableBufferSequence buf
                         , boost::system::error_code& ec)
    {
      //if( buf.data() != in_.rdbuf() )
      //  return in_.read(buf.begin(), buf.size());
      //return buf.size();
      
      //std::cerr<< "In stdio::read_some()" << std::endl
      //         << "before = {" << std::endl
      //         << std::string(boost::asio::buffer_cast<char *>(buf), boost::asio::buffer_size(buf)) << std::endl
      //         << "}" << std::endl;
      std::cin.read(boost::asio::buffer_cast<char *>(buf)
                   , boost::asio::buffer_size(buf));
      if (std::cin.fail() && !std::cin.eof())
      {
        ec = boost::system::error_code(654, boost::system::system_category);
        return 0;
      }
      //std::cerr<< "Read data" << std::endl
      //         << "after = {" << std::endl
      //         << std::string(boost::asio::buffer_cast<char *>(buf), boost::asio::buffer_size(buf)) << std::endl
      //         << "}" << std::endl;
      return std::cin.gcount();
    }

    template<typename ConstBufferSequence>
    std::size_t write_some(ConstBufferSequence& buf
                          , boost::system::error_code& ec)
    {
      std::size_t bytes_transferred(0);
      for(typename ConstBufferSequence::const_iterator i = buf.begin()
         ; i != buf.end(); ++i)
      {
        std::size_t buf_len = boost::asio::buffer_size(*i);
        std::string s(boost::asio::buffer_cast<const char*>(*i)
                     , buf_len);
        bytes_transferred += buf_len;
        std::cout.write(s.c_str(), buf_len);
      }
      return bytes_transferred;
    }

  protected:
    bool is_open_;
  };

  typedef basic_connection<tags::stdio> stdio_connection;

  //  template<typename ProtocolService = detail::cgi_service>
  //struct stdio_connection
  //{
  //  typedef basic_connection<tags::stdio, ProtocolService>    type;
  //};

} // namespace cgi

#endif // CGI_STDIO_CONNECTION_IMPL_HPP_INCLUDED__
