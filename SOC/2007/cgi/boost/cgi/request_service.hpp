//               -- request_service.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_REQUEST_SERVICE_HPP_INCLUDED
#define CGI_REQUEST_SERVICE_HPP_INCLUDED

namespace cgi {

  template<typename Protocol>
  class request_service
    : public boost::asio::io_service::service
  {
  public:
    typedef Protocol                                  protocol_type;
    typedef basic_protocol_service<Protocol>  protocol_service_type;
    typedef basic_request<Protocol>                    request_type;
    //typedef request_impl<Protocol>


    request_service(io_service_type& ios)
      : boost::asio::io_service::service(ios.io_service())
      , io_service_(ios)
    {
    }

    void shutdown_service()
    {
    }

    //void construct

    bool load()
    {
    }

    template<typename ProtocolService, typename Handler>
    class load_handler
    {
    public:
      load_handler(ProtocolService& ps, Handler handler)
        : service_(ps)
        , handler_(handler)
      {
      }

      void operator()(boost::system::error_code& ec)
      {
        if( ec )
          handler(ec);


        //service_.dispatch(handler_(ec));
      }

    private:
      ProtocolService& service_;
      Handler handler_;
    };

    template<typename Handler>
    void async_load(Handler handler)
    {
    }
  private:
    io_service_type& io_service_;
  };

  template<>
  class request_service<protocol::cgi>
  {
  public:

  };

} // namespace cgi

#endif // CGI_REQUEST_SERVICE_HPP_INCLUDED
