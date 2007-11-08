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
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/system/error_code.hpp>

//#include "is_async.hpp"
#include "boost/cgi/io_service.hpp"
#include "boost/cgi/detail/throw_error.hpp"
#include "boost/cgi/detail/protocol_traits.hpp"
#include "boost/cgi/basic_protocol_service_fwd.hpp"
#include "boost/cgi/detail/service_base.hpp"
//#include "service_selector.hpp"
#include "boost/cgi/scgi/request.hpp"

namespace cgi {
 namespace scgi {
  
   /// The service_impl class for SCGI basic_request_acceptor<>s
   /**
    * Note: this is near enough to being generic. It will hopefully translate
    *       directly to the fcgi_acceptor_service_impl. In other words you would
    *       then have one acceptor_service_impl<>, so you'd use
    *       acceptor_service_impl<scgi> acceptor_service_impl_; // and
    *       acceptor_service_impl<fcgi> acceptor_service_impl_; // etc...
    *
    * Note: If the protocol is an asynchronous protocol, which means it requires
    * access to a boost::asio::io_service instance, then this class becomes a
    * model of the Service concept (**LINK**) and must only use the constructor
    * which takes a ProtocolService (**LINK**). If the protocol isn't async then
    * the class can be used without a ProtocolService.
    */
   template<typename Protocol = ::cgi::scgi_>
   class acceptor_service_impl
     : public detail::service_base<request_service<Protocol> >
   {
   public:

     /// The unique service identifier
     //static boost::asio::io_service::id id;
 
     struct implementation_type
     {
       typedef Protocol                              protocol_type;
       typedef basic_protocol_service<protocol_type> protocol_service_type;
       typedef boost::asio::ip::tcp                  native_protocol_type;
       typedef scgi::request                         request_type;
       typedef boost::asio::socket_acceptor_service<
                 native_protocol_type>               acceptor_service_type;
 
       acceptor_service_type::implementation_type    acceptor_;
       boost::mutex                                  mutex_;
       std::queue<boost::shared_ptr<request_type> >  waiting_requests_;
       protocol_service_type*                        service_;
     }; 

     //typedef scgi_request_acceptor_impl                  implementation_type;
     typedef acceptor_service_impl<Protocol>                       type;
     typedef typename type::implementation_type::protocol_type
                                                         protocol_type;
     typedef typename type::implementation_type::protocol_service_type
                                                         protocol_service_type;
     typedef typename type::implementation_type::acceptor_service_type
                                                         acceptor_service_type;
     typedef typename type::implementation_type::native_protocol_type
                                                         native_protocol_type;
     typedef typename acceptor_service_type::native_type native_type;
     //typedef typename acceptor_service_type::native_type native_type;
 
     explicit acceptor_service_impl(::cgi::io_service& ios)
       : detail::service_base<request_service<Protocol> >(ios)
       , acceptor_service_(boost::asio::use_service<acceptor_service_type>(ios))
     {
     }
 
     void set_protocol_service(implementation_type& impl
                              , protocol_service_type& ps)
     {
       impl.protocol_service_ = &ps;
     }

    protocol_service_type& 
      get_protocol_service(implementation_type& impl)
    {
      BOOST_ASSERT(impl.service_ != NULL);
      return *impl.service_;
    }

    void construct(implementation_type& impl)
    {
      acceptor_service_.construct(impl.acceptor_);
      //impl.acceptor_ptr().reset(impl::acceptor_type(this->io_service()));
    }

    void destroy(implementation_type& impl)
    {
      // close/reject all the waiting requests
      /***/
      acceptor_service_.destroy(impl.acceptor_);
    }

    void shutdown_service()
    {
      acceptor_service_.shutdown_service();
    }

    /// Check if the given implementation is open.
    bool is_open(implementation_type& impl)
    {
      return acceptor_service_.is_open(impl.acceptor_);
    }

    /// Open a new *socket* acceptor implementation.
    boost::system::error_code
      open(implementation_type& impl, const native_protocol_type& protocol
          , boost::system::error_code& ec)
    {
      return acceptor_service_.open(impl.acceptor_, protocol, ec);
    }

    /// Assign an existing native acceptor to a *socket* acceptor.
    boost::system::error_code
      assign(implementation_type& impl, const native_protocol_type& protocol
            , const native_type& native_acceptor
            , boost::system::error_code& ec)
    {
      return acceptor_service_.assign(impl.acceptor_, protocol, native_acceptor
                                     , ec);
    }    

    /// Accepts one request.
    template<typename CommonGatewayRequest>
    boost::system::error_code
      accept(implementation_type& impl, CommonGatewayRequest& request
            , boost::system::error_code& ec)
    {
      {
        boost::mutex::scoped_lock lk(impl.mutex_);
        if (!impl.waiting_requests_.empty())
        {
          request = *(impl.waiting_requests_.front());
          impl.waiting_requests_.pop();
          return ec;
        }
      }
      return impl.acceptor_.accept(request.client().connection(), ec);
    }

    /// Asynchronously accepts one request.
    template<typename CommonGatewayRequest, typename Handler>
    void async_accept(implementation_type& impl, CommonGatewayRequest& request
                     , Handler handler, boost::system::error_code& ec)
    {
      this->io_service().post(
        boost::bind(&scgi_request_acceptor_service::check_for_waiting_request
                   , boost::ref(impl), boost::ref(request), handler, ec));      
    }
  private:
    template<typename CommonGatewayRequest, typename Handler>
    void check_for_waiting_request(implementation_type& impl
                                  , CommonGatewayRequest& request
                                  , Handler handler)
    {
      {
        boost::mutex::scoped_lock lk(impl.mutex_);
        if (!impl.waiting_requests_.empty())
        {
          request = *(impl.waiting_requests_.front());
          impl.waiting_requests_.pop();
          return handler(ec); // this could be `io_service::post`ed again
        }
      }
      return accceptor_service_.async_accept(request.client().connection()
                                            , handler);
    }

  private:
    /// The underlying socket acceptor service.
    acceptor_service_type& acceptor_service_;
  };

 } // namespace scgi
} // namespace cgi

#include "boost/cgi/detail/pop_options.hpp"

#endif // CGI_SCGI_ACCEPTOR_SERVICE_IMPL_HPP_INCLUDED__
