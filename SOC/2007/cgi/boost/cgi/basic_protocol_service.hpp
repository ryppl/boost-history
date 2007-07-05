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
#include <boost/asio/io_service.hpp>

#include "basic_gateway.hpp"
#include "connection.hpp"

namespace cgi {

  /// Basic Protocol Service
  /**
   * Holds the request queue and the connection queue.
   * It is also a wrapper around asio::io_service
   */
  template<typename Protocol, typename TypeTraits = type_traits<Protocol> >
  class basic_protocol_service
  {
  public:
    typedef basic_gateway<Protocol>   gateway_type;
    typedef basic_acceptor<Protocol> acceptor_type;
    typedef basic_request<Protocol>   request_type;


    basic_protocol_service()
      : io_service_()
      , mutex_()
      , gateway_(this)
    {
    }

    ~basic_protocol_service()
    {
      gateway_.stop_all();
    }

    void run()
    {
      io_service_.run();
    }

    void stop()
    {
      gateway_.stop_all();
      io_service_.stop();
    }

    boost::asio::io_service& io_service()
    {
      return io_service_;
    }

    template<typename Handler>
    void post(Handler handler)
    {
      io_service_.post(handler);
    }

    template<typename Handler>
    void dispatch(Handler handler)
    {
      io_service_.dispatch(handler);
    }

  /*
    void add_request(request_ptr new_request)
    {
      boost::thread::mutex::scoped_lock lk(mutex_);
      requests_.push_back(new_request);
    }

    bool del_request(request_ptr request)
    {
      boost::thread::mutex::scoped_lock lk(mutex_);
      if( !requests_.find(request) == requests_.end() )
        return false;
  **/

  private:
    boost::asio::io_service io_service_;
    boost::thread::mutex mutex_;
    boost::thread::condition condition_;

    /// A strand is used for guaranteeing handlers are dispatched sequentially
    boost::asio::strand strand_;

    std::set<request_type::pointer> request_set_;
    std::queue<request_type::pointer> request_queue_;
    //std::set<connection_ptr> connections_;
    gateway_type gateway_;
    
    friend class gateway_type;
    friend class gateway_service<protocol_type>;
    friend class acceptor_type;
    friend class request_type;
  };

} // namespace cgi

#endif // CGI_BASIC_PROTOCOL_SERVICE_HPP_INCLUDED__
