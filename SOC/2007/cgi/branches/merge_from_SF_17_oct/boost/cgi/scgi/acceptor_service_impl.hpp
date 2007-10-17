//           -- scgi/acceptor_service_impl.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_SCGI_ACCEPTOR_SERVICE_IMPL_HPP_INCLUDED__
#define CGI_SCGI_ACCEPTOR_SERVICE_IMPL_HPP_INCLUDED__

#include "boost/cgi/detail/push_options.hpp"

#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/utility/enable_if.hpp>

//#include "is_async.hpp"
#include "boost/cgi/io_service.hpp"
#include "boost/cgi/detail/throw_error.hpp"
#include "boost/cgi/detail/protocol_traits.hpp"
#include "boost/cgi/basic_protocol_service_fwd.hpp"
#include "boost/cgi/detail/service_base.hpp"
//#include "service_selector.hpp"

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
  class scgi_request_acceptor_service
    : public detail::service_base<request_service<Protocol> >
  {
  public:
    //typedef typename service_impl_type::impl_type     impl_type;

    typedef scgi_request_acceptor_impl                  implementation_type;
    typedef implementation_type::protocol_type          protocol_type;
    typedef basic_protocol_service<protocol_type>       protocol_service_type;

    /// The unique service identifier
    //static boost::asio::io_service::id id;

    scgi_request_acceptor_service(cgi::io_service& ios)
      : detail::service_base<request_service<Protocol> >(ios)
    {

    }

    void construct(implementation_type& impl)
    {
      impl.acceptor_ptr().reset(impl::acceptor_type(this->io_service()));
    }

    void destroy(implementation_type& impl)
    {
      impl.acceptor().close();
    }

    void shutdown_service()
    {
    }

    bool is_open(implementation_type& impl)
    {
      return impl.is_open();
    }

    template<typename Protocol>
    boost::system::error_code& open(implementation_type& impl
                                   , Protocol& protocol
                                   , boost::system::error_code& ec)
    {
      return impl.acceptor().open(protocol, ec);
    }

    

    template<typename CommonGatewayRequest>
    boost::system::error_code& accept(implementation_type& impl
                                     , CommonGatewayRequest& request
                                     , boost::system::error_code& ec)
    {
      {
        boost::thread::mutex::scoped_lock lk(impl.mutex());
        if (!impl.waiting_requests().empty())
        {
          request = *(impl.waiting_requests().front());
          impl.waiting_requests().pop();
          return ec;
        }
      }
      return impl.acceptor().accept(request.client(), ec);
    }

    template<typename CommonGatewayRequest, typename Handler>
    void async_accept(implementation_type& impl, CommonGatewayRequest& request
                     , Handler handler, boost::system::error_code& ec)
    {
      this->io_service().post(
        boost::bind(&scgi_request_acceptor_service::check_for_waiting_request
                   , boost::ref(impl), boost::ref(request), handler, ec));      
    }
  private:
    //boost::asio::ip::tcp::acceptor acceptor_;

    template<typename CommonGatewayRequest, typename Handler>
    void check_for_waiting_request(implementation_type& impl
                                  , CommonGatewayRequest& request
                                  , Handler handler)
    {
      {
        boost::thread::mutex::scoped_lock lk(impl.mutex());
        if (!impl.waiting_requests().empty())
        {
          request = *(impl.waiting_requests().front());
          impl.waiting_requests().pop();
          return handler(ec); // this could be `io_service::post`ed again
        }
      }
      return impl.accceptor().async_accept(request.client(), handler);
  };

} // namespace cgi

#include "boost/cgi/detail/pop_options.hpp"

#endif // CGI_SCGI_ACCEPTOR_SERVICE_IMPL_HPP_INCLUDED__
