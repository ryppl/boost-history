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

#include "detail/request_traits.hpp"

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
   *
   * Note: From 10/07/07 this class is going to require a protocol_service be
   * passed to it. This is good for FastCGI/SCGI and asynchronous CGI. A full
   * specialisation of this class for sync CGI can be provided... The reasoning
   * for this is that the added complexity for allowing a non-async CGI request
   * to use the acceptor and this class doesn't seem worth it: it makes the
   * whole library much simpler to do it this way.
   */
  template<typename Protocol
          , typename Service = request_traits<Protocol>::service_type
          , enum Role = role_type::responder
          , typename ProtocolService = basic_protocol_service<Protocol>
	        , typename Allocator = std::allocator<char> >
  class basic_request
    : public request_base
      //, public boost::asio::basic_io_object<Service>
    , private boost::noncopyable
  {
  public:
    typedef basic_request<Protocol, Service, Role
                         , ProtocolService, Allocator >  type;
    typedef Protocol                                     protocol_type;
    typedef Role                                         role_type;
    typedef ProtocolService                              protocol_service_type;
    typedef boost::shared_ptr<type>                      pointer;

    typedef Service                                 service_type;
    typedef Service::impl_type                      impl_type;
    typedef boost::shared_ptr<connection_base>      connection_ptr;


    explicit basic_request(protocol_service_type& s, bool load_now = false)
      :// boost::asio::basic_io_object<Service>(s.io_service())
       next_layer_(s)
      , service_(boost::asio::use_service<Service>(next_layer_.io_service()))
      , impl_(service_.null())
      , status_num_(0)
    {
      service_.construct(impl_);
      if( load_now ) service_.load(impl_);
    }

    ~basic_request()
    {
      service_.destroy(impl_);
    }

    boost::asio::io_service& io_service()
    {
      return next_layer_.io_service();
    }

    /// Synchronously read/parse the request meta-data
    /**
     * Note: 'loading' including reading/parsing STDIN if parse_stdin == true
     */
    // Throwing semantics
    void load(bool parse_stdin = false)
    {
      boost::system::error_code ec;
      service_.load(impl_, parse_stdin);
      boost::throw_error(ec);
    }

    // Error-code semantics
    boost::system::error_code& load(boost::system::error_code& ec
				   , bool parse_stdin = false)
    {
      return service_.load(impl_, parse_stdin, ec);
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

      service_.set_status(request_status_ = status_type::ended;
      http_status_ = http_status;
      program_status_ = program_status;
      service_.end(impl_, http_status);
      //io_service_.dispatch(set_status(status_type::ended));
      //impl_.end_request(this, status_code);
    }

    void close(int http_status, int program_status)
    {

    }

    /// Reject the request with a standard '500 Internal Server Error' error
    void reject()
    {
      service_.set_status(impl_, status_type::aborted);
      service_.end(impl_, http::status_code::internal_server_error);
    }

    /// Read some data from the request
    template<typename MutableBufferSequence>
    std::size_t read_some(const MutableBufferSequence& buf)
    {
      error_code ec;
      std::size_t s = service.read_some(buf, ec);
      boost::throw_error(ec);
      return s;
    }

    template<typename MutableBufferSequence>
    std::size_t read_some(const MutableBufferSequence& buf
                         , boost::system::error_code& ec)
    {
      return service.read_some(buf, ec);
    }

    template<typename ConstBufferSequence>
    std::size_t write_some(const ConstBufferSequence& buf)
    {
      error_code ec;
      std::size_t s = service.write_some(buf, ec);
      boost::throw_error(ec);
      return s;
    }

    template<typename ConstBufferSequence>
    std::size_t write_some(const ConstBufferSequence& buf
                          , boost::system::error_code& ec)
    {
      return service.write_some(buf, ec);
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
      return service_.meta_get(impl_, name);
    }

    /// Find the post meta-variable matching name
    /**
     * @param greedy This determines whether more data can be read to find
     * the variable. The default is true to cause least surprise in the common
     * case of having not parsed any of the POST data.
     */
    std::string meta_post(const std::string& name, bool greedy = true)
    {
      return service_.meta_post(impl_, name, greedy);
    }

    /// Find the cookie meta-variable matching name
    std::string meta_cookie(const std::string& name) const
    {
      return service_.meta_cookie(impl_, name);
    }

    /// Find the environment meta-variable matching name
    std::string meta_env(const std::string& name) const
    {
      return service_.meta_env(impl_, name);
    }

    /// Search through all meta vars for the meta-variable matching name
    /**
     * The policy w.r.t. POST data (ie. whether it should all
     * be read/parsed and included in this search or not) is 
     * to be decided.
     *
     * Notes:
     * One option is to parse everything, making this option
     * very inefficient.
     * Another is to leave this function as a 'lazy' search:
     * it'll search with what it's got and no more. Then, also
     * provide a meta_var_all() function which is greedy; the
     * ugly/long name there to discourage use.
     */
    std::string meta_var(const std::string& name, bool greedy = false) const
    {
      std::string request_method( meta_env("REQUEST_METHOD") );

      std::string tmp;

      // If it's not a POST request search meta_get first
      if (request_method.empty() || request_method == "GET")
      {
        tmp = meta_get(name);
        if (!tmp.empty())
          return tmp;
      }

      tmp = meta_cookie(name);
      if (!tmp.empty())
	return tmp;

      tmp = meta_env(name);
      if (!tmp.empty())
	return tmp;

      if (!request_method.empty() && request_method == "POST")
      {
        tmp = meta_post(name);
        if (!tmp.empty())
          return tmp;
      }

      tmp = meta_get(name);
      return tmp.empty() ? "" : tmp;
    }

    // Some helper functions for the basic CGI 1.1 meta-variables
    std::string auth_type()
    { return service_.meta_env(impl_, "AUTH_TYPE"); }

    std::string content_length()
    { return service_.meta_env(impl_, "CONTENT_LENGTH"); }

    std::string content_type()
    { return service_.meta_env(impl_, "CONTENT_TYPE"); }

    std::string gateway_interface()
    { return service_.meta_env(impl_, "GATEWAY_INTERFACE"); }

    std::string path_info()
    { return service_.meta_env(impl_, "PATH_INFO"); }

    std::string path_translated()
    { return service_.meta_env(impl_, "PATH_TRANSLATED"); }

    std::string query_string()
    { return service_.meta_env(impl_, "QUERY_STRING"); }

    std::string remote_addr()
    { return service_.meta_env(impl_, "REMOTE_ADDR"); }

    std::string remote_host()
    { return service_.meta_env(impl_, "REMOTE_HOST"); }

    std::string remote_ident()
    { return service_.meta_env(impl_, "REMOTE_IDENT"); }

    std::string remote_user()
    { return service_.meta_env(impl_, "REMOTE_USER"); }

    std::string request_method()
    { return service_.meta_env(impl_, "REQUEST_METHOD"); }

    std::string script_name()
    { return service_.meta_env(impl_, "SCRIPT_NAME"); }

    std::string server_name()
    { return service_.meta_env(impl_, "SERVER_NAME"); }

    std::string server_port()
    { return service_.meta_env(impl_, "SERVER_PORT"); }

    std::string server_protocol()
    { return service_.meta_env(impl_, "SERVER_PROTOCOL"); }

    std::string server_software()
    { return service_.meta_env(impl_, "SERVER_SOFTWARE"); }


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
      return service_.get_role(impl_);
    }

    /// Get the strand associated with the request (if any)
    // Not sure if the strand concept should be kept separate or a member
    // function like basic_request<>::wrap() should be provided: in the case of
    // a synchronous request type the wrapping would still function as expected
    // and there would be no need for protocol-specific code in user programs.
      /*    boost::asio::strand* strand()
    {
      return impl_.strand();
    }
      */

    /// Get the implementation type for the request
    impl_type& impl() const
    {
      return impl_;
    }

    void set_status(http::status_code status)
    {
      service_.set_http_status(impl_, status);
    }

    bool is_open()
    {
      return service_.is_open(impl_);
    }

  private:
    //connection_ptr conn_;
    service_type& service_;
    impl_type impl_;

    //int app_status_; // what to return to the server on request completion
    //http::status_code http_status_;
    //status_type request_status_;

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
