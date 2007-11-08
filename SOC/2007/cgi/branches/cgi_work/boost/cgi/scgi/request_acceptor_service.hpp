//               -- request_service.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_REQUEST_SERVICE_HPP_INCLUDED
#define CGI_REQUEST_SERVICE_HPP_INCLUDED
#include "boost/cgi/detail/push_options.hpp"

#include <boost/utility/enable_if.hpp>

//#include "is_async.hpp"
#include "boost/cgi/io_service.hpp"
#include "boost/cgi/detail/throw_error.hpp"
#include "boost/cgi/detail/protocol_traits.hpp"
#include "boost/cgi/basic_protocol_service_fwd.hpp"
#include "boost/cgi/detail/service_base.hpp"
//#include "service_selector.hpp"
#include "boost/cgi/scgi/acceptor_service_impl.hpp"

namespace cgi {

  /// The service class for SCGI basic_request_acceptor<>s
  /**
   * Note: If the protocol is an asynchronous protocol, which means it requires
   * access to a boost::asio::io_service instance, then this class becomes a
   * model of the Service concept (**LINK**) and must only use the constructor
   * which takes a ProtocolService (**LINK**). If the protocol isn't async then
   * the class can be used without a ProtocolService.
   */
  template<typename Protocol = scgi_>
  class scgi_request_acceptor_service
    : public detail::service_base<request_service<Protocol> >
  {
  public:
    //typedef typename service_impl_type::impl_type     impl_type;

    typedef scgi::acceptor_service_impl<>               service_impl_type;
    typedef service_impl_type::implementation_type      implementation_type;
    typedef typename implementation_type::protocol_type          protocol_type;
    typedef basic_protocol_service<protocol_type>       protocol_service_type;

    /// The unique service identifier
    //static boost::asio::io_service::id id;

    scgi_request_acceptor_service(::cgi::io_service& ios)
      : detail::service_base<request_service<Protocol> >(ios)
    {
    }

    void construct(implementation_type& impl)
    {
      service_impl_.construct(impl);
    }

    void destroy(implementation_type& impl)
    {
      service_impl_.destroy(impl);
    }

    void shutdown_service()
    {
      service_impl_.shutdown_service();
    }

    bool is_open(implementation_type& impl)
    {
      return service_impl_.is_open(impl);
    }

    void close(implementation_type& impl)
    {
      return service_impl_.close(impl);
    }

    template<typename CommonGatewayRequest>
    boost::system::error_code&
      accept(implementation_type& impl, CommonGatewayRequest& request
            , boost::system::error_code& ec)
    {
      return service_impl_.accept(impl, request, ec);
    }

    template<typename Handler>
    void async_accept(implementation_type& impl, Handler handler)
    {
      service_impl_.async_accept(impl, handler);
    }
  private:
    service_impl_type service_impl_;
  };

} // namespace cgi

#include "boost/cgi/detail/pop_options.hpp"

#endif // CGI_REQUEST_SERVICE_HPP_INCLUDED
