//                    -- reply.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_REPLY_HPP_INCLUDED__
#define CGI_REPLY_HPP_INCLUDED__

#include <ostream>

#include "detail/buffer_ops.hpp"

namespace cgi {

  class reply
  {
  public:
    /// Default constructor
    /**
     * Creates a shared pointer of a buffer to use internally.
     */
    explicit reply()
      : ostream_(detail::make_buffer())
    {
    }

    /// Construct with a particular buffer
    /**
     * Takes the buffer and uses it internally, does nothing with it on
     * destruction.
     */
    reply(std::streambuf* buf)
      : ostream_(buf)
    {
    }

    /// Construct, taking a buffer from an external source
    /**
     * Takes a buffer from T (can be a model of ProtocolService or CGI_Request)
     * to use internally.
     */
    template<typename T>
    reply(T& t)
      : ostream_(t.make_buffer())
    {
    }

    ~reply()
    {
    }

    std::size_t write(

    template<typename MutableBufferSequence>
    std::size_t write(MutableBufferSequence& buf)
    {
      return ostream_.write(buf.data(), buf.size());
    }


    /// Synchronously flush the reply via the supplied request
    /**
     * This call uses throwing semantics. ie. an exception will be thrown on
     * any failure.
     */
    template<typename CGI_Request>
    void flush(CGI_Request& req)
    {
      req.write(ostream_.rdbuf());
    }

    /// Synchronously flush the reply via the supplied request
    /**
     * This call uses error_code semantics. ie. ec is set if an error occurs.
     */
    template<typename CGI_Request>
    boost::system::error_code& flush(CGI_Request& req
                                   , boost::system::error_code& ec)
    {
      return req.write(ostream_.rdbuf(), ec);
    }

    /// Asynchronously flush the reply through the supplied request
    template<typename CGI_Request, typename Handler>
    void async_flush(CGI_Request& req, Handler handler)
    {
      req.async_write(ostream_.rdbuf(), handler));
    }

    /// Synchronously send the reply via the supplied request
    /**
     * This call uses throwing semantics. ie. an exception will be thrown on
     * any failure.
     */
    template<typename CGI_Request>
    void send(CGI_Request& req)
    {
      req.write(ostream_.rdbuf());
      req.end(
    }

    /// Synchronously send the reply via the supplied request
    /**
     * This call uses error_code semantics. ie. ec is set if an error occurs.
     */
    template<typename CGI_Request>
    boost::system::error_code& send(CGI_Request& req
                                   , boost::system::error_code& ec)
    {
      req.write(ostream_.rdbuf(), ec);
      req.end(status_
    }

    template<typename CGI_Request, typename Handler>
    class send_handler
    {
    public:
      send_handler(CGI_Request& req, int status, Handler handler)
        : request_(req)
        , status_(status)
        , handler_(handler)
      {
      }

      void operator()(boost::system::error_code ec)
      {
        if( !ec )
        {
          request_.close(

      }
    private:
      CGI_Request& request_;
      int status_;
      Handler handler_;
    };

    /// Asynchronously send the reply through the supplied request
    template<typename CGI_Request, typename Handler>
    void async_send(CGI_Request& req, Handler handler)
    {
      req.async_write(ostream_.rdbuf()
                     , send_handler<CGI_Request
                                   , Handler>(req, status_, handler));
    }

    /// Operator<< overload for basic outputting ability
    template<typename T>
    reply& operator<<(reply& rep, const T& t)
    {
      ostream_<< t;
      return rep;
    }

    std::streambuf* rdbuf() const { return ostream_.rdbuf(); }

    void status(int num)
    {
      status_ = num;
      //data_.set_status(num);
    }

    int status() const { return status_; }//data_.get_status(); }

  private:
    int status_;
    std::ostream ostream_;

    //template<typename T>
    //friend reply& operator<<(reply&, const T&);
  };

} // namespace cgi

#endif // CGI_REPLY_HPP_INCLUDED__
