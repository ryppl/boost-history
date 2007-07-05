//              -- cgi_request_impl.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_CGI_REQUEST_IMPL_HPP_INCLUDED__
#define CGI_CGI_REQUEST_IMPL_HPP_INCLUDED__

#include <map>
#include <boost/noncopyable.hpp>

namespace cgi {
 namespace detail {
    typename std::map<std::string,std::string>    map_type;
 } // namespace detail


  /// Implementation for a standard CGI request
  /**
   * Note: This isn't noncopyable since there's no real reason it can't be
   * copied around. Since basic_request is noncopyable, basic copying will be
   * restricted but if someone really wants to copy the data, then they can.
   */
  class cgi_request_impl
  {
  public:
    /// Constructor
    /**
     * Since this request type is synchronous, there is no need for an
     * io_service, so the passed ProtocolService is just ignored.
     */
    template<typename ProtocolService>
    cgi_request_impl(ProtocolService& pserv)
    {
      load(false);
    }

    /// Synchronously read/parse the request meta-data
    /**
     * @param parse_stdin if true then STDIN data is also read/parsed
     */
    bool load(bool parse_stdin)
    {
    }

    /// Asynchronously read/parse the request meta-data
    /**
     * @param parse_stdin if true then STDIN data is also read/parsed
     */
    template<typename Handler>
    void async_load(bool parse_stdin, Handler handler)
    {
      load(parse_stdin);
      handler();
    }

    template<typename ConnectionPtr, typename MutableBufferSequence>
    std::size_t read(ConnectionPtr conn, MutableBufferSequence buf)
    {
      return conn.read(buf);
    }

    template<typename ConnectionPtr, typename MutableBufferSequence
            , typename Handler>
    void async_read(ConnectionPtr conn, MutableBufferSequence buf
                   , Handler handler)
    {
      conn.async_read(buf, handler);
    }

    template<typename ConnectionPtr, typename ConstBufferSequence>
    std::size_t write(ConnectionPtr conn, ConstBufferSequence buf)
    {
      return conn.write(buf);
    }

    template<typename ConnectionPtr, typename ConstBufferSequence
            , typename Handler>
    void async_write(ConnectionPtr conn, ConstBufferSequence buf
                    , Handler handler)
    {
      conn.async_write(buf, handler);
    }

    template<typename VarType> map_type& var() const;

    template<typename VarType>
    const std::string& var(const std::string& name) const
    {
      map_type& meta_data = var<VarType>();
      if((map_type::iterator pos = meta_data.find(name))
           != meta_data.end())
      {
        return *pos;
      }
      return std::string();

      /* Alt:
      if( meta_data.find(name) != meta_data.end() )
        return meta_data[name];
      return std::string();
      **/
    }

    void* strand() const { return NULL; }
    role_type role() const { return role_type::responder; }

  private: // functions
    

  private: // variables
    //map_type env_map_;
    map_type http_map_;
    map_type cookie_map_;
    map_type get_map_;
    map_type post_map_;
  };

  template<> inline const std::string&
  cgi_request_impl::var<tags::ENV>(const std::string& name)
  {
    return ::getenv(name);
  }

  /// Get a request map of all the environment meta-variables (slow)
  /**
   * -- NOT IMPLEMENTED FOR NOW --
   *
   * In the case of a CGI request, the environment meta-data is usually stored
   * in the process environment, which means there is no direct access to all
   * of them as a map_type&. In other words, this function call will have to
   * load all of the variables into memory and then return the map
   */
  //template<> inline cgi_request_impl::map_type&
  //cgi_request_impl::var<tags::ENV>()
  //{
  //  throw std::logic_error("Can't get all environment vars as a map_type&");
  //}

  template<> inline cgi_request_impl::map_type&
  cgi_request_impl::var<tags::HTTP>() { return http_map_; }

  template<> inline cgi_request_impl::map_type&
  cgi_request_impl::var<tags::COOKIE>() { return cookie_map_; }

  template<> inline cgi_request_impl::map_type&
  cgi_request_impl::var<tags::GET>() { return get_map_; }

  template<> inline cgi_request_impl::map_type&
  cgi_request_impl::var<tags::POST>() { return post_map_; }

} // namespace cgi

#endif // CGI_CGI_REQUEST_IMPL_HPP_INCLUDED__
