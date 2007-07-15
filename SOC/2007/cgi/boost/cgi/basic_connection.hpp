//                -- basic_connection.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_BASIC_CONNECTION_HPP_INCLUDED__
#define CGI_BASIC_CONNECTION_HPP_INCLUDED__

#include <istream>
#include <ostream>
#include <boost/asio.hpp>
#include "connection_base.hpp"

namespace cgi {

  template< typename ConnectionType, typename ProtocolService>
  class basic_connection
  : connection_base
  //, connection_wrapper<ConnectionType>
  {
  public:
    typedef connection_impl<ConnectionType
                           , ProtocolService::protocol_type>  impl_type;
    typedef boost::shared_ptr<conection_base>                 pointer;

    explicit basic_connection(protocol_service_type& ps)
      : impl_(ps)
    {
    }

    static pointer create()
    {
      return new basic_connection<protocol_service_type
                                 , connection_type>(impl_.protocol_service());
    }

  private:
    impl_type impl_;
  };


  typedef basic_connection<tags::tcp>      tcp_connection;
  //typedef basic_connection<tags::pipe>     pipe_connection; // not implemented


} // namespace cgi

#endif // CGI_BASIC_CONNECTION_HPP_INCLUDED__
