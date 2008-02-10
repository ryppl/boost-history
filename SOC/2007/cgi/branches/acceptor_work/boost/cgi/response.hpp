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
#include <fstream> // only for testing

//#include "boost/cgi/request_ostream.hpp"
#include "boost/cgi/buffer.hpp"
#include "boost/cgi/cookie.hpp"
#include "boost/cgi/header.hpp"
#include "boost/cgi/write.hpp"
#include "boost/cgi/basic_request_fwd.hpp"
#include "boost/cgi/http/status_code.hpp"
#include "boost/cgi/streambuf.hpp"
#include <boost/foreach.hpp>

/// This mess outputs a default Content-type header if the user hasn't set any.
/**
 * BOOST_CGI_ADD_DEFAULT_HEADER should not persiste beyond this file.
 *
 * It basically works like (default first):
 *
 * Debug mode:
 * - Append a "Content-type: text/plain" header;
 * - If BOOST_CGI_DEFAULT_CONTENT_TYPE is defined, set that as the
 *   content-type;
 * - If BOOST_CGI_NO_DEFAULT_CONTENT_TYPE is defined, do nothing.
 *
 * Release mode:
 * - Do nothing.
 */
#if !defined(NDEBUG) && !defined(BOOST_CGI_NO_DEFAULT_CONTENT_TYPE)
//{
#  if !defined(BOOST_CGI_DEFAULT_CONTENT_TYPE)
#    define BOOST_CGI_DEFAULT_CONTENT_TYPE "Content-type: text/plain"
#  endif // !defined(BOOST_CGI_DEFAULT_CONTENT_TYPE)
//}
#  define BOOST_CGI_ADD_DEFAULT_HEADER   \
      if (headers_.empty())              \
        headers_.push_back(BOOST_CGI_DEFAULT_CONTENT_TYPE"\r\n");
#else
#  define BOOST_CGI_ADD_DEFAULT_HEADER
#endif // !defined(NDEBUG) && !defined(BOOST_CGI_NO_DEFAULT_CONTENT_TYPE)


namespace cgi {

  /// The response class: a helper for responding to requests.
  class response
  {
  public:
    typedef std::ostream ostream_type;

    response(http::status_code sc = http::ok)
      : buffer_(new ::cgi::streambuf())
      , ostream_(buffer_.get())
      , http_status_(sc)
      , headers_terminated_(false)
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

    ~response()
    {
    } 

    /// Clear the response buffer.
    void clear()
    {
      ostream_.clear();
      headers_.clear();
    }

    /// Return the response to the 'just constructed' state.
    void reset()
    {
      clear();
      headers_terminated_ = false;
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
      return ostream_.write(buf.begin(), buf.end());
      //return buf.size();
    }

    /// Synchronously flush the data to the supplied SyncWriteStream
    /**
     * This call uses throwing semantics. ie. an exception will be thrown on
     * any failure.
     * If there is no error, the buffer is cleared.
     */
    template<typename SyncWriteStream>
    void flush(SyncWriteStream& sws)
    {
      boost::system::error_code ec;
      flush(sws, ec);
      detail::throw_error(ec);
    }
    
    /// Synchronously flush the data via the supplied request
    /**
     * This call uses error_code semantics. ie. ec is set if an error occurs.
     * If there is no error, the buffer is cleared.
     */
    template<typename SyncWriteStream>
    boost::system::error_code
      flush(SyncWriteStream& sws, boost::system::error_code& ec)
    {
      if (!headers_terminated_)
      {
        std::vector<boost::asio::const_buffer> headers;
        prepare_headers(headers);//, ec);
        common::write(sws, headers, boost::asio::transfer_all(), ec);
        if (ec)
          return ec;
      }

      std::size_t bytes_written = common::write(sws, buffer_->data(), ec);
      if (!ec)
        buffer_->consume(bytes_written);

      return ec;
    }

    /// Synchronously send the data via the supplied request.
    /**
     * This call uses throwing semantics. ie. an exception will be thrown on
     * any failure.
     * Note: The data in the stream isn't cleared after this call.
     */
    template<typename SyncWriteStream>
    void send(SyncWriteStream& sws)
    {
      boost::system::error_code ec;
      send(sws, ec);
      detail::throw_error(ec);
    }

    /// Synchronously send the data via the supplied request.
    /**
     * This call will not throw, but will set `ec` such that `ec == true` if
     * an error occurs. Details of the error are held in the `error_code`
     * object.
     */
    template<typename SyncWriteStream>
    boost::system::error_code&
      send(SyncWriteStream& sws, boost::system::error_code& ec)
    {
      if (!headers_terminated_)
      {
        //BOOST_FOREACH(headers_.begin(), headers_.end()
        //             , headers.push_back(::cgi::buffer(*_1)));
        std::vector<boost::asio::const_buffer> headers;
        prepare_headers(headers);//, ec)
        if (common::write(sws, headers, boost::asio::transfer_all(), ec))
          std::cerr<< "Broken." << std::endl;
          //return ec;
      }

      common::write(sws, buffer_->data(), boost::asio::transfer_all(), ec);

      return ec;
    }

    /// Asynchronously send the data through the supplied request
    /**
     * Note: This is quite crude at the moment and not as asynchronous as
     *       it could/should be. The data in the stream isn't cleared after
     *       this call.
     */
    template<typename AsyncWriteStream, typename Handler>
    void async_send(AsyncWriteStream& aws, Handler handler)
    {
      aws.io_service().post(
        boost::bind(&response::do_async_send, aws, handler)
      );
    }

    template<typename AsyncWriteStream, typename Handler>
    void do_async_send(AsyncWriteStream& aws, Handler handler)
    {


      //req.set_status(http_status_);
      /*
      if (!headers_terminated_)
      {
        ostream_<< "Content-type: text/plain\r\n\r\n";
        headers_terminated_ = true;
      }
      */
      common::async_write(aws, rdbuf()->data(), handler);
    }

    /// Get the buffer associated with the stream
    common::streambuf*
      rdbuf()
    {
      return static_cast<common::streambuf*>(ostream_.rdbuf());
    }

    /// Set the status code associated with the response.
    response& set_status(const http::status_code& num)
    {
      http_status_ = num;
      return *this;
    }

    /// Get the status code associated with the response.
    http::status_code& get_status()
    {
      return http_status_;
    }

    http::status_code& status()
    {
      return http_status_;
    }

    /// Allow more headers to be added (WARNING: avoid using this).
    void unterminate_headers()
    {
      headers_terminated_ = false;
    }

    /// Get the length of the body of the response
    std::size_t content_length()
    {
      return rdbuf()->size();
    }

    /// Add a header after appending the CRLF sequence.
    response& set_header(const std::string& value)
    {
      BOOST_ASSERT(!headers_terminated_);
      headers_.push_back(value + "\r\n");
      return *this;
    }

    /// Format and add a header given name and value, appending CRLF.
    response& set_header(const std::string& name, const std::string& value)
    {
      BOOST_ASSERT(!headers_terminated_);
      headers_.push_back(name + ": " + value + "\r\n");
      return *this;
    }

    void clear_headers()
    {
      BOOST_ASSERT(!headers_terminated_);
      headers_.clear();
    }

    void reset_headers()
    {
      headers_.clear();
      headers_terminated_ = false;
    }

    bool headers_terminated() const
    {
      return headers_terminated_;
    }
  protected:
    // Vector of all the headers, each followed by a CRLF
    std::vector<std::string> headers_;

    boost::shared_ptr<common::streambuf> buffer_; // maybe scoped_ptr?

    ostream_type ostream_;

    http::status_code http_status_;

    // True if no more headers can be appended. 
    bool headers_terminated_;

    template<typename T>
    friend response& operator<<(response& resp, const T& t);

  private:

    // Send the response headers and mark that they've been sent.
    template<typename ConstBufferSequence>
    //boost::system::error_code
    void
      prepare_headers(ConstBufferSequence& headers)//, boost::system::error_code& ec)
    {
      BOOST_CGI_ADD_DEFAULT_HEADER

      // Terminate the headers.
      if (!headers_terminated_)
        headers_.push_back("\r\n");

      //{ Construct a ConstBufferSequence out of the headers we have.
      //std::vector<boost::asio::const_buffer> headers;
      typedef std::vector<std::string>::iterator iter;
      for (iter i(headers_.begin()), e(headers_.end()); i != e; ++i)
      {
        headers.push_back(common::buffer(*i));
      }
      //}

      headers_terminated_ = true;
      //return ec;
    }
   };

  /// Generic ostream template
  template<typename T>
  response& operator<<(response& resp, const T& t)
  {
    resp.ostream_<< t;
    return resp;
  }

  /// You can stream a cgi::header into a response.
  /**
   * This is just a more convenient way of doing:
   *
   * ``
   * resp.set_header(header_content)
   * ``
   *
   * [tip
   * If you stream a default-constructed header to a response, it
   * 'terminates' the headers. ie. You can do this if you want to ensure
   * no further headers are added to the response. It has no other side-
   * effects; for instance, it won't write any data to the client.
   * ]
   * /
  template<typename T>
  response& operator<<(response& resp, const ::cgi::basic_header<std::basic_string<T> >& hdr)
  {
    if (hdr.content.empty()) {
      resp.headers_terminated_ = true;
      return resp;
    }else{
      // We shouldn't allow headers to be sent after they're explicitly ended.
      BOOST_ASSERT(!resp.headers_terminated_);
      resp.set_header(hdr.content);
      return resp;
    }
  }*/

  template<>
  response& operator<<(response& resp, const ::cgi::header& hdr)
  {
    if (hdr.content.empty()) {
      resp.headers_terminated_ = true;
      return resp;
    }else{
      // We shouldn't allow headers to be sent after they're explicitly ended.
      BOOST_ASSERT(!resp.headers_terminated_);
      resp.set_header(hdr.content);
      return resp;
    }
  }

  /// You can stream a cgi::cookie into a response.
  /**
   * This is just a shorthand way of setting a header that will set a
   * client-side cookie.
   *
   * You cannot stream a cookie to a response after the headers have been
   * terminated. In this case, an alternative could be to use the HTML tag:
   * <meta http-equiv="Set-cookie" ...> (see http://tinyurl.com/3bxftv or
   * http://tinyurl.com/33znkj), but this is outside the scope of this
   * library.
   */
  template<typename T>
  response& operator<<(response& resp, basic_cookie<T> ck)
  {
    BOOST_ASSERT(!resp.headers_terminated());
    resp.set_header("Set-cookie", ck.to_string());
    return resp;
  }

} // namespace cgi

#undef BOOST_CGI_ADD_DEFAULT_HEADER

#include "boost/cgi/detail/pop_options.hpp"

#endif // CGI_RESPONSE_HPP_INCLUDED__
