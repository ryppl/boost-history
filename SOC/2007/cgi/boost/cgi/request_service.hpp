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

  /// The generic service class for basic_request<>s
  /**
   * Note: If the protocol is an asynchronous protocol, which means it requires
   * access to a boost::asio::io_service instance, then this class becomes a
   * model of the Service concept (**LINK**) and must only use the constructor
   * which takes a ProtocolService (**LINK**). If the protocol isn't async then
   * the class can be used without a ProtocolService.
   */
  template<typename Protocol>
  class request_service
    : public boost::enable_if<is_async<request_traits<Protocol
                                                     >::service_impl_type
                                      >::value
                             , detail::service_base<request_service>
                             >::type
  {
    // The platform-specific implementation (only one for now)
    typedef detail::request_traits<Protocol>::service_impl_type
      service_impl_type;

  public:
    typedef service_impl_type::impl_type                  impl_type;
  
    typedef Protocol                                  protocol_type;
    typedef basic_protocol_service<Protocol>  protocol_service_type;
    typedef basic_request<Protocol>                    request_type;
    //typedef request_impl<Protocol>


    /// The unique service identifier
    //static boost::asio::io_service::id id;

    request_service(protocol_service_type& ps)
      : boost::asio::io_service::service(ps.io_service())
      , service_impl_(boost::asio::use_service<service_impl_type>(ps.io_service()))
    {
    }

    void create(impl_type& impl)
    {
      service_impl_.create(impl);
    }

    void destroy(impl_type& impl)
    {
      service_impl_.destroy(impl);
    }

    void shutdown_service()
    {
      service_impl_.shutdown_service();
    }

    impl_type null() const
    {
      return service_impl_.null();
    }

    //void construct

    boost::system::error_code& load(bool parse_stdin, boost::system::error_code& ec)
    {
      return service_impl_.load(parse_stdin, ec);
    }


    template<typename Handler>
    void async_load(impl_type& impl, bool parse_stdin, boost::system::error_code& ec
                   , Handler handler)
    {
      service_impl_.async_load(impl, parse_stdin, ec, handler);
    }

    bool is_open(impl_type& impl);
    {
      return service_impl_.is_open(impl);
    }

  private:
    service_impl_type& service_impl_;
  };

} // namespace cgi

#endif // CGI_REQUEST_SERVICE_HPP_INCLUDED
