//                   -- response.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_RESPONSE_HPP_INCLUDED__
#define CGI_RESPONSE_HPP_INCLUDED__

#include "boost/cgi/detail/push_options.hpp"

#include <string>

//#include "boost/cgi/request_ostream.hpp"
#include "boost/cgi/buffer.hpp"
#include "boost/cgi/cookie.hpp"
#include "boost/cgi/header.hpp"

namespace cgi {

  /// The response class: a helper for responding to requests

  // The request_ostream is destined to become a basic_request_ostream
//typedef request_ostream<> response_;


  class response
  {
  public:
    response(http::status_code sc = http::ok)
      : buffer_(new ::cgi::streambuf())
      , ostream_(buffer_.get())
      , http_status_(sc)
      , headers_sent_(false)
    {
    }

    /// Construct with a particular buffer
    /**
     * Takes the buffer and uses it internally, does nothing with it on
     * destruction.
     */
    response(::cgi::streambuf* buf, http::status_code sc = http::ok)
      : /*request_(NULL)
          , */ostream_(buf)
      , http_status_(sc)
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
//    template<typename CommonGatewayRequest>
//    request_ostream(CommonGatewayRequest& req, http::status_code sc = http::ok)
//      : request_(&req)
//      , buffer_(new cgi::streambuf())
//      , ostream_(buffer_.get()) //detail::take_buffer(req))
//      , http_status_(sc)
////    , destination_(destination)
//    {
//    }

    ~response()
    {
    } 

    void clear()
    {
      ostream_.clear();
      headers_.clear();
    }

    // provide this too?
    std::size_t write(const char* str, std::size_t len)
    {
      ostream_.write(str, len);
      return len;
    }

    std::size_t write(const std::string& str)
    {
      return write(str.c_str(), str.size());
    }

    template<typename ConstBufferSequence>
    std::size_t write(const ConstBufferSequence& buf)
    {
      //ostream_.write(buf.data(), buf.size());
      return buf.size();
    }

    /// Synchronously flush the data to the current request
    /**
     * If there is no error, the buffer is cleared.
     */
    //void flush()
    //{
    //  BOOST_ASSERT(request_ != NULL);
    //  flush(*request_);
    //}

    /// Synchronously flush the data to the supplied request
    /**
     * This call uses throwing semantics. ie. an exception will be thrown on
     * any failure.
     * If there is no error, the buffer is cleared.
     */
    template<typename CommonGatewayRequest>
    void flush(CommonGatewayRequest& req)
    {
      /*
      if (!headers_sent_)
      {
        ostream_<< "Content-type: text/plain\r\n\r\n";
        headers_sent_ = true;
      }
      */
      ::cgi::write(req, headers_);
      ::cgi::write(req, rdbuf()->data());
      // the above function will throw on an error
      clear();
    }

    /// Synchronously flush the data via the supplied request
    /**
     * This call uses error_code semantics. ie. ec is set if an error occurs.
     * If there is no error, the buffer is cleared.
     */
    template<typename CommonGatewayRequest>
    boost::system::error_code&
      flush(CommonGatewayRequest& req, boost::system::error_code& ec)
    {
      /*
      if (!headers_sent_)
      {
        ostream_<< "Content-type: text/plain\r\n\r\n";
        headers_sent_ = true;
      }
      */
      if(!::cgi::write(req, rdbuf()->data(), ec))
        clear();
      return ec;
    }

    // Class for doing post-flush housekeeping (ie. clearing the stream data)
    template<typename Handler>
    class flush_handler
    {
    public:
      flush_handler(response& os, Handler handler)
        : ostream_(os)
        , handler_(handler)
      {
      }

      void operator()(boost::system::error_code& ec)
      {
        if(!ec) ostream_.clear();
        handler_(ec);
      }
    private:
      response& ostream_;
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
      /*
      if (!headers_sent_)
      {
        ostream_<< "Content-type: text/plain\r\n\r\n";
        headers_sent_ = true;
      }
      */
      ::cgi::async_write(req, rdbuf()->data()
                        , flush_handler<Handler>
                            (*this, handler, boost::arg<1>()));
    }


    /// Synchronously send the reply to the default request
    /**
     * Note: The data in the stream isn't cleared after this call, but the
     * request held in the ostream is removed. ie. send() can't be called
     * twice without an arguement (unless you add another request - something
     * not possible yet).
     */
    //void send()
    //{
    //  BOOST_ASSERT(request_ != NULL);
    //  send(*request_);
    //  request_ = NULL;
    //}

    /// Synchronously send the reply to the default request
    /**
     * Note: The data in the stream isn't cleared after this call. If the send
     * is sucessful, the request held in the ostream is removed. ie. send()
     * can't be called twice without an arguement (unless you add another
     * request - something not possible yet).
     */
    //boost::system::error_code& send(boost::system::error_code& ec)
    //{
    //  BOOST_ASSERT(request_ != NULL);
    //  if(!send(*request_, ec))
    //    request_ = NULL;
    //  return ec;
    //}


    /// Synchronously send the data via the supplied request
    /**
     * This call uses throwing semantics. ie. an exception will be thrown on
     * any failure.
     * Note: The data in the stream isn't cleared after this call.
     */
    template<typename CommonGatewayRequest>
    void send(CommonGatewayRequest& req)
    {
      /*
      if (!headers_sent_)
      {
        ostream_<< "Content-type: text/plain\r\n\r\n";
        headers_sent_ = true;
      }
      */
      ::cgi::write(req.client(), rdbuf()->data());
      req.set_status(http_status_);
    }

    /// Synchronously send the data via the supplied request
    /**
     * This call uses error_code semantics. ie. ec is set if an error occurs.
     * Note: The data in the stream isn't cleared after this call.
     */
    template<typename CommonGatewayRequest>
    boost::system::error_code&
      send(CommonGatewayRequest& req, boost::system::error_code& ec)
    {
      /*
      if (!headers_sent_)
      {
        ostream_<< "Content-type: text/plain\r\n\r\n";
        headers_sent_ = true;
      }
      */
      ::cgi::write(req.client(), rdbuf()->data(), ec);
      req.set_status(http_status_);
      return ec;
    }

    /// Asynchronously send the data through the supplied request
    /**
     * Note: The data in the stream isn't cleared after this call.
     */
    template<typename CommonGatewayRequest, typename Handler>
    void async_send(CommonGatewayRequest& req, Handler handler)
    {
      req.set_status(http_status_);
      /*
      if (!headers_sent_)
      {
        ostream_<< "Content-type: text/plain\r\n\r\n";
        headers_sent_ = true;
      }
      */
      ::cgi::async_write(req, rdbuf()->data(), handler);
    }

    /// Get the buffer associated with the stream
    ::cgi::streambuf*
      rdbuf()
    {
      return static_cast<::cgi::streambuf*>(ostream_.rdbuf());
    }

    void set_status(const http::status_code& num)
    {
      http_status_ = num;
    }

    http::status_code& get_status()
    {
      return http_status_;
    }

    void set_header(const std::string& value)
    {
      headers_.push_back(value);
    }

    void set_header(const std::string& name, const std::string& value)
    {
      headers_.push_back(name + ": " + value + "\r\n");
    }
  protected:
    std::vector<std::string> headers_;
    boost::shared_ptr<::cgi::streambuf> buffer_;
    std::ostream ostream_;
    http::status_code http_status_;
    bool headers_sent_;

    template<typename T>
    friend response& operator<<(response& resp, const T& t);

    //template<typename T>
    //friend response& operator<<(response& resp, T t);

    /// Some helper functions for the basic CGI 1.1 meta-variables
//     void auth_type(const std::string& value)
//     {
//       std::string str("Auth-type: ");
//       str += value;
//       this->headers_.push_back(cgi::buffer(str.c_str(), str.size()));
//     }
// 
//     void content_length(const std::string& value)
//     {
//       std::string str("Content-length: ");
//       str += value;
//       this->headers_.push_back(cgi::buffer(str.c_str(), str.size()));
//     }

  };

  template<typename T>
  response& operator<<(response& resp, const T& t)
  {
    resp.ostream_<< t;
    return resp;
  }

  //template<typename T>
  //response& operator<<(response& resp, T t)
  //{
  //  resp.ostream_<< t;
  //  return resp;
  //}

  template<typename T>
  response& operator<<(response& resp, const header& hdr)
  {
    if (hdr.content.empty()) {
      resp.headers_sent_ = true;
      return resp<< "\r\n";
    }else{
      resp.headers_.push_back(hdr.content);
      return resp;
    }
  }

  template<typename T>
  response& operator<<(response& resp, const basic_cookie<T>& ck)
  {
    // Note: the 'set-cookie' isn't part of the cookie object since
    // the cookie can also be set after the headers have been sent.
    // See http://tinyurl.com/33znkj
    return resp<< "Set-cookie: " << ck.to_string() << "\r\n";
  }

} // namespace cgi

#include "boost/cgi/detail/pop_options.hpp"

#endif // CGI_RESPONSE_HPP_INCLUDED__
