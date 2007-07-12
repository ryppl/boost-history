//              -- stdio_connection_impl.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_STDIO_CONNECTION_IMPL_HPP_INCLUDED__
#define CGI_STDIO_CONNECTION_IMPL_HPP_INCLUDED__

namespace cgi {

  template<typename ProtocolService>
  class basic_connection<tags::stdio>
    : public connection_base
  {
  public:
    typedef ProtocolService   protocol_service_type;

    basic_connection()
      : in_(std::cin)
      , out_(std::cout)
    {
    }

    basic_connection(protocol_service_type&)
      : in_(std::cin)
      , out_(std::cout)
    {
    }

    template<typename MutableBufferSequence>
    std::size_t read_some(MutableBufferSequence buf, boost::system::error_code& ec)
    {
      if( buf.data() != in_.rdbuf() )
        return in_.read(buf.data(), buf.size());
      return buf.size();
    }

    template<typename ConstBufferSequence>
    std::size_t write_some(ConstBufferSequence& buf, boost::system::error_code& ec)
    {
      return out_.write(buf.data(), buf.size());
    }
      
  private:
    std::istream& in_;
    std::ostream& out_;
  };

  typedef basic_connection<tags::stdio>    stdio_connection;

} // namespace cgi

#endif // CGI_STDIO_CONNECTION_IMPL_HPP_INCLUDED__