//                -- basic_request.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_BASIC_REQUEST_HPP_INCLUDED__
#define CGI_BASIC_REQUEST_HPP_INCLUDED__

#include <iostream>
#include <boost/noncopyable.hpp>

namespace cgi {

  enum status_type
    { ok
    , aborted
    , ended
    };


  /// The basic_request class, primary entry point to the library
  /**
   * Note: This class is supposed to make simple use of the library easy.
   * This comes with some restrictions, such as being noncopyable, and also
   * providing copies of the meta-variables rather than references. This makes
   * sure the internal data is protected.
   * The underlying impl_type classes aren't like this (and as such aren't
   * quite as 'safe' to use) but they may be more suited to certain needs.
   *
   * Note: This class isn't thread safe: carrying around a mutex-per-request
   * seems prohibitively expensive. There could be functions which take a mutex
   * as an arguement and lock it. (Async calls could get messy if you need a
   * protected request object)
   */
  template<typename Protocol
          , enum RequestRole = role_type::responder
          , typename ProtocolService = basic_protocol_service<Protocol>
	        , typename Allocator = std::allocator<char> >
  class basic_request
    : public request_base
    , private boost::noncopyable
  {
  public:
    typedef basic_request<Protocol, RequestRole
                         , ProtocolService, Allocator >  type;
    typedef Protocol                                     protocol_type;
    typedef RequestRole                                  role_type;
    typedef ProtocolService                              protocol_service_type;
    //typedef meta_data<protocol_type>                     data_type;
    typedef boost::shared_ptr<type>                      pointer;

    typedef request_traits<Protocol>                traits;
    typedef typename traits::impl_type              impl_type;
    typedef typename traits::service_type           service_type;
    typedef boost::shared_ptr<connection_base>      connection_ptr;


    explicit basic_request(protocol_service_type& s, bool load_now = false)
      : impl_(s)
      , status_num_(0)
      //, strand_(s.io_service())
    {
      if( load_now ) load();
    }

    /// Alternate constructor for CGI requests, which don't require a service
    explicit basic_request(bool load_now = false)
      : impl_()
      , status_num_(0)
    {
      if( load_now ) load();
    }

    ~basic_request()
    {
      end(status_num_);
    }

    /// Synchronously read/parse the request meta-data
    /**
     * Note: 'loading' including reading/parsing STDIN if parse_stdin == true
     */
    bool load(bool parse_stdin = false)
    {
      if( !service_.load(impl_, parse_stdin) )
      {
        status_ = status_type::aborted;     
        return false;
      }
      return true;
    }

    /// Asynchronously read/parse the request meta-data
    /**
     * Note: 'loading' including reading/parsing STDIN if parse_stdin == true
     */
    template<typename Handler>
    void async_load(Handler handler, bool parse_stdin = false)
    {
      service_.async_load(impl_, parse_stdin, handler);
    }

    /// Read num_bytes into a default buffer (post-data buffer)
    std::size_t read(std::size_t num_bytes)
    {
      std::size_t bytes_read = impl_.read(connection_, buffer());
      // buff.commit(bytes_read)
      return bytes_read;
    }

    /// Read data synchronously into the provided buffer
    /**
     * @return the number of bytes read
     */
    template<typename MutableBufferSequence>
    std::size_t read(MutableBufferSequence& buf)
    {
      return impl_.read(connection_, buf);
    }

    /// Read data asynchronously into the provided buffer
    template<typename MutableBufferSequence, typename Handler>
    void async_read(MutableBufferSequence& buf, Handler handler)
    {
      impl_.async_read(connection_, buf, handler);
    }

    template<typename ConstBufferSequence>
    std::size_t write(ConstBufferSequence& buf)
    {
      return impl_.async_write(connection_, buf);
    }

    template<typename ConstBufferSequence, typename Handler>
    void async_write(ConstBufferSequence& buf, Handler handler)
    {
      impl_.async_write(connection_, buf, handler);
    }

    /// Notify the server the request has finished being handled
    /**
     * In certain situations (such as a Proactor client using the async read
     * functions) it will be necessary to call end, rather than just returning
     * from the sub_main function.
     *
     * @param program_status This value is returned to the server indicating the
     * state of the request after it was finished handling. It is
     * implementation defined how the server deals with this, and it may have
     * no effect on the http status code returned to the client (eg. 200 OK).
     */
    void close(http::status_code http_status, int program_status)
    {
      BOOST_ASSERT( request_status_ != status_type::ended );

      request_status_ = status_type::ended;
      http_status_ = http_status;
      program_status_ = program_status;
      service_.end(impl_);
      //io_service_.dispatch(set_status(status_type::ended));
      //impl_.end_request(this, status_code);
    }

    void close(int http_status, int program_status)
    {

    }

/* Not sure if these are needed; leaving it for open discussion to decide.

    template<class VarType = ENV>
    const std::string& var(const std::string& name) const
    {
      return impl_.var<VarType>(name);
    }
    
    template<typename ToType, class VarType = ENV>
    const std::string& var_as(const std::string& name) const
    {
      return boost::lexical_cast<ToType>(var<VarType>(name));
    }
**/

    /// Find the get meta-variable matching name
    std::string meta_get(const std::string& name) const
    {
      return impl_.var<tags::GET>(name);
    }

    /// Find the post meta-variable matching name
    /**
     * @param greedy This determines whether more data can be read to find
     * the variable. The default is true to cause least surprise in the common
     * case of having not parsed any of the POST data.
     */
    std::string meta_post(const std::string& name, bool greedy = true)
    {
      std::string& value = impl_.var<POST>(name);
      if( value.empty() && greedy && impl_.post_data_read() )
        return service_.var<tags::POST>(name);

      return value;
    }

    /// Find the cookie meta-variable matching name
    std::string meta_cookie(const std::string& name) const
    {
      return impl_.var<tags::COOKIE>(name);
    }

    /// Find the environment meta-variable matching name
    std::string meta_env(const std::string& name) const
    {
      return impl_.var<tags::ENV>(name);
    }

    /// Search through all meta vars for the meta-variable matching name
    /**
     * The policy w.r.t. POST data (ie. whether it should all
     * be read/parsed and included in this search or not) is 
     * to be decided.
     *
     * Notes:
     * One option is to parse everything, making this option
     * a very poor one in production applications.
     * Another is to leave this function as a 'lazy' search:
     * it'll search with what it's got and no more. Then, also
     * provide a meta_var_all() function which is greedy; the
     * ugly/long name there to discourage use.
     */
    std::string meta_var(const std::string& name) const
    {
      std::string request_method( meta_env("REQUEST_METHOD") );

      std::string tmp;

      // If it's not a POST request search meta_get first
      if( request_method.empty() || request_method == "GET" )
      {
	      tmp = meta_get(name);
	      if( !tmp.empty() )
	        return tmp;
      }

      tmp = meta_cookie(name);
      if( !tmp.empty() ) return tmp;
      tmp = meta_env(name);
      if( !tmp.empty() ) return tmp;

      if( !request_method.empty() && request_method == "POST" )
      {
        tmp = meta_post(name);
        if( !tmp.empty() )
          return tmp;
      }

      tmp = meta_get(name);
      return tmp.empty() ? "" : tmp;
    }

    // Some helper functions for the basic CGI 1.1 meta-variables
    std::string auth_type()
    { return impl_.var<tags::ENV>("AUTH_TYPE"); }

    std::string content_length()
    { return impl_.var<tags::ENV>("CONTENT_LENGTH"); }

    std::string content_type()
    { return impl_.var<tags::ENV>("CONTENT_TYPE"); }

    std::string gateway_interface()
    { return impl_.var<tags::ENV>("GATEWAY_INTERFACE"); }

    std::string path_info()
    { return impl_.var<tags::ENV>("PATH_INFO"); }

    std::string path_translated()
    { return impl_.var<tags::ENV>("PATH_TRANSLATED"); }

    std::string query_string()
    { return impl_.var<tags::ENV>("QUERY_STRING"); }

    std::string remote_addr()
    { return impl_.var<tags::ENV>("REMOTE_ADDR"); }

    std::string remote_host()
    { return impl_.var<tags::ENV>("REMOTE_HOST"); }

    std::string remote_ident()
    { return impl_.var<tags::ENV>("REMOTE_IDENT"); }

    std::string remote_user()
    { return impl_.var<tags::ENV>("REMOTE_USER"); }

    std::string request_method()
    { return impl_.var<tags::ENV>("REQUEST_METHOD"); }

    std::string script_name()
    { return impl_.var<tags::ENV>("SCRIPT_NAME"); }

    std::string server_name()
    { return impl_.var<tags::ENV>("SERVER_NAME"); }

    std::string server_port()
    { return impl_.var<tags::ENV>("SERVER_PORT"); }

    std::string server_protocol()
    { return impl_.var<tags::ENV>("SERVER_PROTOCOL"); }

    std::string server_software()
    { return impl_.var<tags::ENV>("SERVER_SOFTWARE"); }


    /// The role that the request is playing
    /**
     * The default role type is responder.
     *
     * In some cases - for instance with FastCGI - the role type can be
     * different
     * eg. authorizer, or filter.
     */
    role_type& role()
    {
      return impl_.role();
    }

    /// Get the strand associated with the request (if any)
    // Not sure if the strand concept should be kept separate or a member
    // function like basic_request<>::wrap() should be provided: in the case of
    // a synchronous request type the wrapping would still function as expected
    // and there would be no need for protocol-specific code in user programs.
    boost::asio::strand* strand()
    {
      return impl_.strand();
    }

    /// Get the implementation type for the request
    impl_type& impl() const
    {
      return impl_;
    }

  private:
    connection_ptr conn_;
    service_type& service_;
    impl_type impl_;

    int app_status_; // what to return to the server on request completion
    http_status http_status_;
    status_type request_status_;

    //boost::shared_ptr<data_type> data_;

    /// The data for the request is held in its own struct
		//data_type* data_;
		
		/// The actual data held by the request
    //std::auto_ptr<meta_data> data_;


  };




  //template<typename T>
  //std::string& basic_request<T>::var<GET>(const std::string& name) const
  //{
  //  return data_.get_var(name);
  //}

  //template<typename T>
  //std::string& basic_request<T>::var<POST>(const std::string& name) const
  //{
  //  return data_.post_var(name);
  //}

  //template<typename T>
  //std::string& basic_request<T>::var<COOKIE>(const std::string& name) const
  //{
  //  return data_.cookie_var(name);
  //}


  //template<>
  //class basic_request<protocol::cgi>::meta_data
  //{
  //public:
  //  meta_data()
  //    : bytes_left_(-1)
  //  {}

  //  std::string get_var(const std::string& name)
  //  {
  //    if( get_vars_.find(name) != get_vars_.end() )
  //      return get_vars_[name];
  //    return "";
  //  }
  //  std::string post_var(const std::string& name)
  //  {
  //    assert( getenv("REQUEST_METHOD") == "POST" );

  //    // see if we already have the value stored
  //    if( post_vars_.find(name) != post_vars_.end() )
  //      return post_vars_[name];
  //    // else check if there's more data to read
  //    while( !read_all_data() )
  //    {
  //      switch( read_var(name) )
  //      {
  //      case true:
  //        return post_vars_[name];
  //      case false:
  //        break;
  //      default:
  //        continue;
  //      }
  //    }
  //    return "";
  //  }
  //  std::string cookie_var(const std::string& name)
  //  {
  //    return getenv(name);
  //  }
  //  std::string env_var(const std::string& name)
  //  {
  //    return getenv(name);
  //  }
  //private:
  //  /// Read a name/value pair from stdin
  //  /**
  //   * @return true if the name (of the n/v pair) read == name
  //   */
  //  bool read_var(std::string& name)
  //  {
  //    char ch;
  //    std::string n;
  //    std::string v;
  //    while( std::cin.get(ch) && bytes_left_-- )
  //    {


  //    std::getline(std::cin, n, '=');
  //    std::getline(std::cin, v, '&');


  //  int bytes_left_;
  //  var_map get_vars_;
  //  var_map post_vars_;
  //};

  //template<>
  //class basic_request<protocol::fcgi>::meta_data
  //{
  //public:
  //  std::string get_var(const std::string& name)
  //  std::string post_var(const std::string& name)
  //  std::string cookie_var(const std::string& name)
  //  std::string env_var(const std::string& name)
  //private:
  //};

  //template<>
  //class basic_request<protocol::scgi>::meta_data
  //{
  //  std::string get_var(const std::string& name)
  //  std::string post_var(const std::string& name)
  //  std::string cookie_var(const std::string& name)
  //  std::string env_var(const std::string& name)
  //};

} // namespace cgi

#endif // CGI_BASIC_REQUEST_HPP_INCLUDED__

/*
NOTES:
* When a request is aborted (eg. when the client closes the connection)
  the library can call an abort_handler() function stored in the request.
  - The user should supply an abort_handler-derived function if they want
    any special handling of aborted requests
*/
