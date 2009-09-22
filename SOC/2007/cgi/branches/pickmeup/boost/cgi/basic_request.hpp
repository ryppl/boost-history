//                -- basic_request.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
//
// Defines the basic_request<> class; the main entry-point to the
// library.
//
////////////////////////////////////////////////////////////////
#ifndef CGI_BASIC_REQUEST_HPP_INCLUDED__
#define CGI_BASIC_REQUEST_HPP_INCLUDED__

#include "boost/cgi/detail/push_options.hpp"

#include <boost/mpl/if.hpp>
#include <boost/assert.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/functional/hash.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/basic_io_object.hpp>
///////////////////////////////////////////////////////////
#include "boost/cgi/common/map.hpp"
#include "boost/cgi/common/is_async.hpp"
#include "boost/cgi/common/path_info.hpp"
#include "boost/cgi/common/role_type.hpp"
#include "boost/cgi/common/source_enums.hpp"
#include "boost/cgi/common/request_service.hpp"
#include "boost/cgi/http/status_code.hpp"
#include "boost/cgi/fwd/basic_request_fwd.hpp"
#include "boost/cgi/fwd/basic_protocol_service_fwd.hpp"
#include "boost/cgi/import/basic_io_object.hpp"
#include "boost/cgi/detail/basic_sync_io_object.hpp"
#include "boost/cgi/detail/throw_error.hpp"
#include "boost/cgi/detail/protocol_traits.hpp"

#ifndef BOOST_CGI_POST_MAX
    /// Restrict POST data to less than 7MB per request.
#   define BOOST_CGI_POST_MAX 6663322
#endif // BOOST_CGI_POST_MAX

namespace cgi {
 namespace common {

   /// Get a hashed interpretation of the request.
   /**
    * You cannot consider this completely unique to each
    * request, but it should be quite useful anyway.
    * You can use this for logging or tracking, for example.
    */
   template<typename T, typename S>
   std::size_t hash_value(basic_request<T,S> const& req);

  /// The basic_request class, primary entry point to the library
  /**
   * Note: By default, synchronous protocols (ie. cgi) auto-load AND parse
   * STDIN,whereas async protocols don't.
   *
   * Note: The alternative functions which take a boost::system::error_code are
   * the non-throwing versions. Instead of a boost::system::system_error being
   * thrown in case of an error, the passed error_code will be set to the value
   * of the error, s.t. if (error) evaluates to true.`
   * 
   * Note: This class isn't thread safe: carrying around a mutex-per-request
   * seems prohibitively expensive. There could be functions which take a mutex
   * as an argument and lock it. (Async calls could get messy if you need a
   * protected request object).
  **/
  template<typename RequestService
          , typename ProtocolService
          , typename Allocator>
  class basic_request
    : public boost::mpl::if_<
                 is_async<typename RequestService::protocol_type>
               , basic_io_object<RequestService>
                 // A basic_sync_io_object is a little more lightweight
                 // than a basic_io_object. It can't handle asynchonous
                 // operations as a consequence.
               , detail::basic_sync_io_object<RequestService>
               >::type
  {
  public:
    typedef
    basic_request<
        RequestService, ProtocolService
      , Allocator
    >                                                    self_type;
    typedef ::cgi::common::map                           map_type;
    typedef RequestService                               service_type;
    typedef typename service_type::protocol_type         protocol_type;
    typedef ProtocolService                              protocol_service_type;
    typedef boost::shared_ptr<self_type>                 pointer;
    typedef typename RequestService::implementation_type implementation_type;
    typedef typename implementation_type::char_type      char_type;
    typedef typename implementation_type::string_type    string_type;
    typedef typename implementation_type::client_type    client_type;
    typedef typename implementation_type::buffer_type    buffer_type;

    /// A proxy class to provide access to the data maps as member variables.
    /**
     * This wraps the underlying data map and exposes a std::map-like interface
     * for the different data maps.
     *
     * It also includes an as<> member function which casts the found data into
     * any type the user specifies.
     */
    template<typename MapType>
    struct data_map_proxy
    {
      typedef MapType                                   map_type;
      typedef typename map_type::key_type               key_type;
      typedef typename map_type::value_type             value_type;
      typedef typename map_type::mapped_type            mapped_type;
      typedef typename map_type::size_type              size_type;
      typedef typename map_type::iterator               iterator;
      typedef typename map_type::const_iterator         const_iterator;
      typedef typename map_type::reverse_iterator       reverse_iterator;
      typedef typename map_type::const_reverse_iterator const_reverse_iterator;
      typedef typename map_type::allocator_type         allocator_type;
      
      void set(map_type& data) { impl = &data; }
      
      iterator begin() {
         BOOST_ASSERT(impl); return impl->begin(); }
      iterator end() {
         BOOST_ASSERT(impl); return impl->end(); }
      const_iterator begin() const {
         BOOST_ASSERT(impl); return impl->begin(); }
      const_iterator end() const {
         BOOST_ASSERT(impl); return impl->end(); }
      reverse_iterator rbegin() {
         BOOST_ASSERT(impl); return impl->rbegin(); }
      reverse_iterator rend() {
         BOOST_ASSERT(impl); return impl->rend(); }
      const_reverse_iterator rbegin() const {
         BOOST_ASSERT(impl); return impl->rbegin(); }
      const_reverse_iterator rend() const {
         BOOST_ASSERT(impl); return impl->rend(); }

      bool empty() { BOOST_ASSERT(impl); return impl->empty(); }
      
      void clear() { BOOST_ASSERT(impl); return impl->clear(); }
      
      size_type size() const { BOOST_ASSERT(impl); return impl->size(); }
      
      size_type count(const key_type& key) {
         BOOST_ASSERT(impl);
         return impl->count(key);
      }
      
      template<typename T>
      T as(key_type const& key) {
        BOOST_ASSERT(impl);
        mapped_type& val((*impl)[key]);
        return val.empty() ? T() : boost::lexical_cast<T>(val);
      }
      
      mapped_type& operator[](key_type const& varname) {
        BOOST_ASSERT(impl); 
        return (*impl)[varname.c_str()];
      }
      
      mapped_type const& operator[](key_type const& varname) const {
        BOOST_ASSERT(impl); 
        return (*impl)[varname.c_str()];
      }
      
      operator map_type&() { BOOST_ASSERT(impl); return *impl; }
      bool operator!() const { return !impl; }

    private:      
      map_type* impl;
    };
    
    data_map_proxy<env_map>    env;
    data_map_proxy<post_map>   post;
    data_map_proxy<get_map>    get;
    data_map_proxy<form_map>   form;
    data_map_proxy<cookie_map> cookies;

    basic_request(const parse_options opts = parse_none
                 , char** base_env = NULL)
      : detail::basic_sync_io_object<service_type>()
    {
      if (opts > parse_none) load(opts, base_env);
      construct();
    }

    // Won't throw
    basic_request(boost::system::error_code& ec
                 , const parse_options opts = parse_none
                 , char** base_env = NULL)
      : detail::basic_sync_io_object<service_type>()
    {
      if (opts > parse_none) load(opts, ec);
    }

    // Throws
    basic_request(protocol_service_type& s
                 , const parse_options opts = parse_none
                 , char** base_env = NULL)
      : basic_io_object<service_type>(s.io_service())
    {
      set_protocol_service(s);
      if (opts > parse_none) load(opts, base_env);
    }

    // Won't throw
    basic_request(protocol_service_type& s
                 , boost::system::error_code& ec
                 , const parse_options opts = parse_none
                 , char** base_env = NULL)
      : basic_io_object<service_type>(s.io_service())
    {
      set_protocol_service(s);
      if (opts > parse_none) load(opts, ec, base_env);
    }

    /// Make a new mutiplexed request from an existing connection.
    // Throws.
    basic_request(implementation_type& impl)
      : basic_io_object<service_type>(impl.service_->io_service())
    {
      set_protocol_service(*impl.service_);
      boost::system::error_code ec;
      this->service.begin_request_helper(this->implementation
                                        , impl.header_buf_, ec);
      detail::throw_error(ec);
    }

    /// Make a new mutiplexed request from an existing connection.
    // Won't throw.
    basic_request(implementation_type& impl, boost::system::error_code& ec)
      : basic_io_object<service_type>(impl.service_->io_service())
    {
      set_protocol_service(*impl.service_);
      this->service.begin_request_helper(this->implementation
                                        , impl.header_buf_, ec);
      construct();
    }
      
    ~basic_request()
    {
      //if (is_open())
      //  close(http::internal_server_error, 0);
    }
    
    void construct()
    {
      this->env.set(env_vars(this->implementation.vars_));
      // By default, set the form map to the environment.
      //if (!this->form)
      //  this->form.set(env_vars(this->implementation.vars_));
      this->post.set(post_vars(this->implementation.vars_));
      this->get.set(get_vars(this->implementation.vars_));
      this->cookies.set(cookie_vars(this->implementation.vars_));
    }

    static pointer create(protocol_service_type& ps)
    {
      return pointer(new self_type(ps));
    }

    void set_protocol_service(protocol_service_type& ps)
    {
      this->service.set_service(this->implementation, ps);
    }

    /// The id of this request.
    /**
     * This is 1 for CGI/aCGI requests, but may be != 1 for FastCGI
     * requests.
     *
     * Note that for FastCGI requests, the id's are assigned on a
     * *per-connection* policy, so in one application you may have 
     * several requests with the same id.
     */
    int id()
    {
      return this->service.request_id(this->implementation);
    }

    /// Check if the request is still open (ie. not aborted or closed)
    bool is_open()
    {
      return this->service.is_open(this->implementation);
    }

    /// Synchronously read/parse the request meta-data
    /**
     * Note: 'loading' including reading/parsing STDIN if
     * parse_stdin == true
     */
    void load(parse_options parse_opts = parse_env, char** base_env = NULL)
    {
      boost::system::error_code ec;
      load(parse_opts, ec, base_env);
      detail::throw_error(ec);
    }

    // Error-code semantics
    boost::system::error_code
      load(parse_options parse_opts, boost::system::error_code& ec
          , char** base_environment = NULL, bool is_command_line = true)
    {
      construct();
      // Parse just the environment first, then check the user
      // isn't trying to upload more data than we want to let them.
      // Define `BOOST_CGI_POST_MAX` to set the maximum content-length
      // allowed.
      if (parse_opts & parse_env)
      {
        //this->service.load(this->implementation, parse_env, ec);
        //if (content_length() >= BOOST_CGI_POST_MAX)
        //  ec = common::error::max_post_exceeded;
        this->service.load(this->implementation, parse_opts, ec);
        // Load the environment passed by the user.
        if (base_environment)
          this->service.load_environment(
              this->implementation, base_environment
            , is_command_line);

        if (parse_opts > parse_env && parse_opts & parse_form)
        {
          common::name rm(request_method().c_str());
          form.set(
            (rm == "GET" || rm == "HEAD") ? get :
                rm == "POST" ? post : env
          );
        }
      }
      return ec;
    }

    void load(char** base_environment, bool is_command_line = true)
    {
      this->service.load_environment(this->implementation
                                    , base_environment
                                    , is_command_line);
    }

    /// Get the buffer containing the POST data.
    /**
     * **FIXME**
     * This actually returns the whole buffer on FastCGI at the moment, 
     * which contains the params too.
     */
    buffer_type& post_buffer()
    {
      return this->implementation.post_buffer_;
    }
    
    // **FIXME**
    /// Asynchronously read/parse the request meta-data
    /**
     * Note: 'loading' including reading/parsing STDIN if
     * parse_stdin == true
     */
    //template<typename Handler>
    //void async_load(Handler handler, bool parse_stdin = false)
    //{
    //  this->service.async_load(this->implementation, parse_stdin
    //                          , handler);
    //}

    /// Notify the server the request has been handled.
    /**
     * In certain situations (such as a Proactor client using the async
     * read functions) it will be necessary to call end, rather than 
     * just returning from the sub_main function.
     *
     * @param program_status This value is returned to the server
     * indicating the state of the request after it was finished 
     * handling. It is implementation defined how the server deals with
     * this, and it may have no effect on the http status code returned
     * to the client (eg. 200 OK).
     *
     * @returns The value of program_status
     */
    int close(common::http::status_code http_status = http::ok
             , int program_status = 0)
    {
      boost::system::error_code ec;
      this->service.close(this->implementation, http_status,
          program_status, ec);
      // Clear the request data so the object can be reused.
      clear();
      detail::throw_error(ec);
      return program_status;
    }

    int close(common::http::status_code http_status
             , int program_status
             , boost::system::error_code& ec)
    {
      return this->service.close(this->implementation, http_status
                                , program_status, ec);
    }

    /// Reject the request with a '500 Internal Server Error' error.
    int reject()
    {
      this->service.set_status(this->implementation, aborted);
      return this->service.close(this->implementation
                                , http::internal_server_error);
    }

    /// Abort a request.
    void abort()
    {
      this->service.set_status(this->implementation, aborted);
    }

    /// Check if a POST variable references a file upload.
    /**
     * File uploads, which originate from a form POSTed using the
     * multipart/form-data encoding type are saved to disk.
     *
     * Only the filename is stored in the POST map for the request.
     * If you expect a field to be a file upload, check if it is using
     * this function.
     */
    bool is_file(common::name const& key)
    {
      return this->service.is_file(this->implementation, key.c_str());
    }

    /// Check if a POST variable references a file upload.
    /**
     * File uploads, which originate from a form POSTed using the
     * multipart/form-data encoding type are saved to disk.
     *
     * Only the filename is stored in the POST map for the request.
     * If you expect a field to be a file upload, check if it is using
     * this function.
     */
    bool is_file(string_type const& key)
    {
      return this->service.is_file(this->implementation, key);
    }

    /// Clear the data for the request, for reusing this object.
    // I'd imagine clearing and re-loading a request is quicker than 
    // destroying/re-creating one. **Unverified claims** **FIXME**
    void clear()
    {
      this->service.clear(this->implementation);
    }

    /// Get the client connection associated with the request
    /**
     * You use the client for read/write calls.
     */
    client_type& client()
    {
      return this->service.client(this->implementation);
    }

    /// Read some data into the request, parsing if necessary.
    void read_some()
    {
      boost::system::error_code ec;
      this->service.read_some(this->implementation, ec);
      detail::throw_error(ec);
    }

    /// Read some data into the request, parsing if necessary.
    std::size_t
    read_some(boost::system::error_code& ec)
    {
      return this->service.read_some(this->implementation, ec);
    }

    /// Read some data into the supplied buffer, parsing if necessary.
    // **FIXME** (remove - use req.client().read_some() instead)
    template<typename MutableBufferSequence>
    void read_some(const MutableBufferSequence& buf)
    {
      boost::system::error_code ec;
      this->service.read_some(this->implementation, buf, ec);
      detail::throw_error(ec);
    }

    /// Read some data into the supplied buffer, parsing if necessary.
    // **FIXME** (remove - use req.client().read_some() instead)
    template<typename MutableBufferSequence>
    std::size_t
    read_some(const MutableBufferSequence& buf
             , boost::system::error_code& ec)
    {
      return this->service.read_some(this->implementation, buf, ec);
    }

    /// Set the output for the request
    /**
     * Not Implemented Yet ******************
     *
     * Set the output sink as `stdout_`, `stderr_`, or `stdout_ | stderr_`
     */
    /*
    void set_output(cgi::sink dest, boost::system::error_code& ec)
    {
      this->service(this->implementation, dest, ec);
    }
    */

    // [helper-functions for the basic CGI 1.1 meta-variables.
    string_type& auth_type()
    { return env["AUTH_TYPE"]; }

    /// Get the content length as a long.
    /**
     * The content length defaults to zero if it isn't explicitly set
     * by your HTTP server.
     */
    long content_length()
    { 
      string_type& cl(env["CONTENT_LENGTH"]);
      return boost::lexical_cast<long>(cl.empty() ? "0" : cl); 
    }

    string_type& content_type()
    { return env["CONTENT_TYPE"]; }

    string_type& gateway_interface()
    { return env["GATEWAY_INTERFACE"]; }

    common::path_info path_info()
    { return env["PATH_INFO"]; }

    string_type& path_translated()
    { return env["PATH_TRANSLATED"]; }

    string_type& query_string()
    { return env["QUERY_STRING"]; }

    string_type& remote_addr()
    { return env["REMOTE_ADDR"]; }

    string_type& remote_host()
    { return env["REMOTE_HOST"]; }

    string_type& remote_ident()
    { return env["REMOTE_IDENT"]; }

    string_type& remote_user()
    { return env["REMOTE_USER"]; }

    string_type& method()
    { return env["REQUEST_METHOD"]; }

    string_type& request_method()
    { return env["REQUEST_METHOD"]; }

    string_type& url()
    { return env["REQUEST_URL"]; }

    string_type& request_url()
    { return env["REQUEST_URL"]; }

    string_type& script_name()
    { return env["SCRIPT_NAME"]; }

    string_type& script_url()
    { return env["SCRIPT_URL"]; }

    string_type& script_uri()
    { return env["SCRIPT_URI"]; }

    string_type& server_name()
    { return env["SERVER_NAME"]; }

    string_type& server_port()
    { return env["SERVER_PORT"]; }

    string_type& server_protocol()
    { return env["SERVER_PROTOCOL"]; }

    string_type& server_software()
    { return env["SERVER_SOFTWARE"]; }

    string_type& referer()
    { return env["HTTP_REFERER"]; }
    // -- end helper-functions]

    /// Get the charset from the CONTENT_TYPE header
    string_type charset()
    {
      // Not sure if regex is needlessly heavy-weight here.
      boost::regex re(";[ ]?charset=([-\\w]+);");
      boost::smatch match;
      if (!boost::regex_match(this->content_type(), match, re))
        return ""; // A default could go here.

      return match[1];
    }

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
      return this->service.get_role(this->implementation);
    }

    std::size_t hash()
    {
      return boost::hash<self_type>()(*this);
    }

    void set_status(common::http::status_code const& status)
    {
      this->service.set_status(this->implementation, status);
    }
  };

   template<typename T, typename S>
   std::size_t hash_value(basic_request<T,S> const& req)
   {
     boost::hash<typename basic_request<T,S>::string_type> hasher;
     return hasher(req.env["REMOTE_ADDR"] + ":"
                 + req.env["REMOTE_PORT"] + ":"
                 + req.env["HTTP_USER_AGENT"] + ":"
                 + req.env["REQUEST_METHOD"] + ":"
                 + req.env["REQUEST_URI"]);
   }

 } // namespace common
} // namespace cgi

#include "boost/cgi/detail/pop_options.hpp"

#endif // CGI_BASIC_REQUEST_HPP_INCLUDED__

/*
NOTES::future_plans:
* When a request is aborted (eg. when the client closes the connection)
  the library can call an abort_handler() function stored in the request.
  - The user should supply an abort_handler-derived function if they want
    any special handling of aborted requests
*/

