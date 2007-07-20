//            -- basic_protocol_service.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_BASIC_PROTOCOL_SERVICE_HPP_INCLUDED__
#define CGI_BASIC_PROTOCOL_SERVICE_HPP_INCLUDED__

#include <set>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>

#include "io_service_provider.hpp"
#include "basic_gateway.hpp"
//#include "basic_connection_fwd.hpp"
#include "basic_acceptor_fwd.hpp"
#include "basic_request_fwd.hpp"

namespace cgi {

  /// Basic Protocol Service
  /**
   * Holds the request queue and the connection queue.
   * It is also a wrapper around asio::io_service
   */
  template<typename Protocol, int IoServiceCount, typename PoolingPolicy>
  class basic_protocol_service
    //: public protocol_traits<Protocol> // do this!
  {
  public:
    typedef Protocol                         protocol_type;
    typedef io_service_provider<IoServiceCount, PoolingPolicy>       
                                             ios_provider_type;
    //typedef protocol_traits<Protocol>      traits;
    //typedef typename traits::gateway_type  gateway_type;
    typedef basic_gateway<Protocol>          gateway_type;
    typedef basic_request<Protocol>          request_type;
    typedef boost::shared_ptr<request_type>  request_ptr;
    typedef basic_acceptor<Protocol>         acceptor_type;


    basic_protocol_service(int pool_size_hint = 0)
      : ios_provider_(pool_size_hint)
//    , mutex_()
      , gateway_(*this)
    {
    }

    basic_protocol_service(boost::asio::io_service& io_service)
      : ios_provider_(io_service)
//    , mutex_()
      , gateway_(*this)
    {
    }

    ~basic_protocol_service()
    {
      gateway_.stop();
    }

    void run()
    {
      ios_provider_.run();
    }

    void stop()
    {
      gateway_.stop();
      ios_provider_.stop();
    }

    /// Return an available io_service from the IoServiceProvider
    boost::asio::io_service& io_service()
    {
      return ios_provider_.io_service();
    }

    /// Post the handler through an available io_service
    template<typename Handler>
    void post(Handler handler)
    {
      ios_provider_.io_service().post(handler);
    }

    /// Dispatch a handler through an available io_service
    template<typename Handler>
    void dispatch(Handler handler)
    {
      ios_provider_.io_service().dispatch(handler);
    }

  private:
    ios_provider_type ios_provider_;
//  boost::thread::mutex mutex_;
//  boost::thread::condition condition_;

    /// A strand is used for guaranteeing handlers are dispatched sequentially
//  boost::asio::strand strand_;

    std::set<request_ptr> request_set_;
    std::queue<request_ptr> request_queue_;

    gateway_type gateway_;

    friend class basic_gateway<protocol_type>;//gateway_type;
    friend class basic_acceptor<protocol_type>;//class acceptor_type;
    friend class basic_request<protocol_type>;//typename request_type;
  };

} // namespace cgi

#endif // CGI_BASIC_PROTOCOL_SERVICE_HPP_INCLUDED__
