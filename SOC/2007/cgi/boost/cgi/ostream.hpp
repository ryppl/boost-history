//                    -- ostream.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_OSTREAM_HPP_INCLUDED__
#define CGI_OSTREAM_HPP_INCLUDED__

/*********************************
ISSUES:
-------
* async_flush()
The other flush() functions clear the buffer after flushing the data to the
request. It's trickier with async_flush(), but for now the buffer is still
cleared after the flush completes. This means you can't write to the ostream
until an async_flush completes; that's probably true of any async operation
however so it seems a reasonable restriction.

* Is ostream the best name for this? request_ostream?

* This class could do more: the functions basic_request<>::write() could write
to the request unbuffered. ie. a write call would instantly wrap the supplied
buffer in suitable headers and send it straight away, rather than buffering it
first. That would also mean the ostream class is of more use than it is now.

* This should probably derive from std::ostream, as users would probably expect
that.
*********************************/

#include <ostream>

#include "detail/take_buffer.hpp"

namespace cgi {

  /// The ostream class: a stream interface for writing to requests
  /**
   * This is simply a wrapper around an ostream with a few extra details that
   * aid writing to a request's output or error sinks.
   *
   * This is a generalisation of the cgi::reply and cgi::logger classes.
   */
  class ostream
    // derive from std::ostream? (yes, basically)
  {
  public:
    /// Default constructor
    explicit ostream(int destination = tags::stdout)
      : request_(NULL)
      , destination_(destination)
    {
    }

    /// Construct with a particular buffer
    /**
     * Takes the buffer and uses it internally, does nothing with it on
     * destruction.
     */
    ostream(std::streambuf* buf, int destination = tags::stdout)
      : ostream_(buf)
      , request_(NULL)
      , destination_(destination)
    {
    }

    /// Construct, taking a buffer from an external source
    /**
     * Gets a buffer from the request/protocol service held by the request.
     * <strike>
     * Takes a buffer from T (can be a model of ProtocolService or
     * CommonGatewayRequest) to use internally.
     * </strike>
     */
    template<typename CommonGatewayRequest>
    ostream(CommonGatewayRequest& req, int destination = tags::stdout)
      : ostream_(detail::take_buffer(req))
      , request_(&req)
      , destination_(destination)
    {
    }

    ~ostream()
    {
      if( request_ ) send();
    }

    void clear()
    {
      ostream_.clear();
    }

    // provide this too?
    std::size_t write(const char* str, std::size_t len)
    {
      return ostream_.write(str, len);
    }

    std::size_t write(const std::string& str)
    {
      return ostream_.write(str);
    }

    template<typename MutableBufferSequence>
    std::size_t write(MutableBufferSequence& buf)
    {
      return ostream_.write(buf.data(), buf.size());
    }

    /// Synchronously flush the data to the current request
    /**
     * If there is no error, the buffer is cleared.
     */
    void flush()
    {
      BOOST_ASSERT(request_ != NULL);
      flush(*request_);
    }

    /// Synchronously flush the data to the supplied request
    /**
     * This call uses throwing semantics. ie. an exception will be thrown on
     * any failure.
     * If there is no error, the buffer is cleared.
     */
    template<typename CommonGatewayRequest>
    void flush(CommonGatewayRequest& req)
    {
      req.write(ostream_.rdbuf());
      clear();
    }

    /// Synchronously flush the data via the supplied request
    /**
     * This call uses error_code semantics. ie. ec is set if an error occurs.
     * If there is no error, the buffer is cleared.
     */
    template<typename CommonGatewayRequest>
    boost::system::error_code& flush(CommonGatewayRequest& req
                                   , boost::system::error_code& ec)
    {
      if(!req.write(ostream_.rdbuf(), ec))
        clear();
      return ec;
    }

    template<typename Handler>
    class flush_handler
    {
    public:
      flush_handler(ostream& os, Handler handler)
        : ostream_(os)
        , handler_(handler)
      {
      }

      void operator()(boost::system::error_code& ec)
      {
        if( !ec )
          ostream_.clear();
        handler(ec);
      }
    private:
      ostream& ostream_;
      Handler handler_;
    };

    /// Asynchronously flush the data through the supplied request
    /**
     * If there is no error, the buffer is cleared *after* the write has
     * finished.
     */
    template<typename CommonGatewayRequest, typename Handler>
    void async_flush(CommonGatewayRequest& req, Handler handler)
    {
      req.async_write(ostream_.rdbuf()
                     , flush_handler<Handler>(*this, handler
                                             , boost::arg<1>)));
    }



    /// Synchronously send the reply to the default request
    /**
     * Note: The data in the stream isn't cleared after this call, but the
     * request held in the ostream is removed. ie. send() can't be called
     * twice without an arguement (unless you add another request - something
     * not possible yet).
     */
    void send()
    {
      BOOST_ASSERT(request_ != NULL);
      send(*request_);
      request_ = NULL;
    }

    /// Synchronously send the reply to the default request
    /**
     * Note: The data in the stream isn't cleared after this call. If the send
     * is sucessful, the request held in the ostream is removed. ie. send()
     * can't be called twice without an arguement (unless you add another
     * request - something not possible yet).
     */
    boost::system::error_code& send(boost::system::error_code& ec)
    {
      BOOST_ASSERT(request_ != NULL);
      if(!send(*request_, destination_, ec))
        request_ = NULL;
      return ec;
    }

    /// Synchronously send the data via the supplied request
    /**
     * This call uses throwing semantics. ie. an exception will be thrown on
     * any failure.
     * Note: The data in the stream isn't cleared after this call.
     */
    template<typename CommonGatewayRequest>
    void send(CommonGatewayRequest& req)
    {
      req.write(ostream_.rdbuf(), destination_);
      req.set_status(http_status_);
    }

    /// Synchronously send the data via the supplied request
    /**
     * This call uses error_code semantics. ie. ec is set if an error occurs.
     * Note: The data in the stream isn't cleared after this call.
     */
    template<typename CommonGatewayRequest>
    boost::system::error_code& send(CommonGatewayRequest& req
                                   , boost::system::error_code& ec)
    {
      req.write(ostream_.rdbuf(), destination_, ec);
      req.set_status(http_status_);
    }

    template<typename CommonGatewayRequest, typename Handler>
    class send_handler
    {
    public:
      send_handler(CommonGatewayRequest& req, http::status_code status
                  , Handler handler)
        : request_(req)
        , http_status_(status)
        , handler_(handler)
      {
      }

      void operator()(boost::system::error_code ec)
      {
        if( !ec )
        {
          request_.set_status(http_status_);

      }
    private:
      CommonGatewayRequest& request_;
      int http_status_;
      Handler handler_;
    };

    /// Asynchronously send the data through the supplied request
    /**
     * Note: The data in the stream isn't cleared after this call.
     */
    template<typename CommonGatewayRequest, typename Handler>
    void async_send(CommonGatewayRequest& req, Handler handler)
    {
      req.async_write(ostream_.rdbuf(), destination_
                     , send_handler<CommonGatewayRequest
                                   , Handler>(req, http_status_, handler));
    }

    /// Get the buffer associated with the 
    std::streambuf* rdbuf() const { return ostream_.rdbuf(); }

    void set_status(http::status_code& num) { http_status_ = num; }
    http::status_code& get_status() const { return http_status_; }

  private:
    http::status_code http_status_;
    std::ostream ostream_;
    int destination_;

    /// The request associated with the ostream; can be NULL
    request_base* request_;

    friend template<typename T> ostream& operator<<(ostream&, const T&);
  };

    /// Operator<< overload for basic outputting ability
    template<typename T>
    ostream& operator<<(ostream& rep, const T& t)
    {
      ostream_<< t;
      return rep;
    }

} // namespace cgi

#endif // CGI_OSTREAM_HPP_INCLUDED__
