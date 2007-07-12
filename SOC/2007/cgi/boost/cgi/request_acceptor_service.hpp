//           -- request_acceptor_service.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_REQUEST_ACCEPTOR_SERVICE_HPP_INCLUDED
#define CGI_REQUEST_ACCEPTOR_SERVICE_HPP_INCLUDED

namespace cgi {

  template<typename Protocol>
  class request_acceptor_service
  {
  public:
    typedef Protocol protocol_type;
    typedef basic_request<Protocol, ...>    implementation_type;

    explicit request_acceptor_service(basic_protocol_service<protocol_type>& s)
      : pservice_(s)
    {
    }

    void shutdown_service()
    {
    }

    void construct(implementation_type& impl)
    {
    }

    void destroy(implementation_type& impl)
    {
    }

    /// Accept a request
    /**
     * Check if there is a waiting request in the queue. If not, accept a
     * connection, and associate it with the request.
     */
    template<typename CommonGatewayRequest>
    boost::system::error_code&
    accept(CommonGatewayRequest& request, boost::system::error_code& ec)
    {
      boost::thread::mutex::scoped_lock lk(io_service_.mutex_);
      if( !io_service_.request_queue_.empty() )
      {
	      request = pservice_.request_queue_.front();
	      pservice_.request_queue_.pop();

	      return ec;
      }
      lk.unlock();
      pservice_.gateway_.accept(&request.connection(), ec);
            
      return ec;
    }

    /// Asynchronously accept a request
    template<typename CommonGatewayRequest, typename Handler>
    void async_accept(CommonGatewayRequest& request, Handler handler)
    {
      boost::thread::mutex::scoped_lock lk(io_service_.mutex_);
      if( !io_service_.request_queue_.empty() )
      {
	      request = pservice_.request_queue_.front();
	      pservice_.request_queue_.pop();

	      return ec;
      }
      lk.unlock();
      pservice_.gateway_.async_accept(request.connection(), handler);
    }

  private:
    basic_protocol_service<protocol_type>& pservice_;
  };

} // namespace cgi

#endif // CGI_REQUEST_ACCEPTOR_SERVICE_HPP_INCLUDED
