//             -- fcgi/request_service.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_FCGI_REQUEST_SERVICE_HPP_INCLUDED__
#define CGI_FCGI_REQUEST_SERVICE_HPP_INCLUDED__

#include <boost/system/error_code.hpp>

//#include "boost/cgi/scgi/request_impl.hpp"
#include "boost/cgi/map.hpp"
#include "boost/cgi/tags.hpp"
#include "boost/cgi/read.hpp"
#include "boost/cgi/role_type.hpp"
#include "boost/cgi/io_service.hpp"
#include "boost/cgi/detail/throw_error.hpp"
#include "boost/cgi/detail/service_base.hpp"
#include "boost/cgi/detail/extract_params.hpp"
#include "boost/cgi/fcgi/client.hpp"

namespace cgi {
 namespace fcgi {

  /// The IoObjectService class for a FCGI basic_request<>s
  class fcgi_request_service
    : public detail::service_base<fcgi_request_service>
  {
  public:
    /// The actual implementation date for an FCGI request.
    struct implementation_type
    {
      typedef ::cgi::map                        map_type;
      //typedef shareable_tcp_connection          connection_type;
      typedef ::cgi::fcgi_                      protocol_type;
      //typedef basic_client<
      //  connection_type, protocol_type
      //>                                         client_type;
      typedef ::cgi::fcgi::client               client_type;
      typedef client_type::connection_type      connection_type;
      typedef std::vector<char>                 buffer_type;
      typedef boost::asio::mutable_buffers_1    mutable_buffers_type;
      typedef client_type::header_buffer_type   header_buffer_type;
      typedef detail::protocol_traits<
        protocol_type
      >::protocol_service_type                  protocol_service_type;

      implementation_type()
        : client_()
        , stdin_parsed_(false)
        , http_status_(http::no_content)
        , request_status_(unloaded)
        , request_role_(spec_detail::ANY)
        , all_done_(false)
      {
      }

      protocol_service_type* service_;

      client_type client_;

      bool stdin_parsed_;
      http::status_code http_status_;
      status_type request_status_;
      fcgi::spec_detail::role_t request_role_;

      buffer_type buffer_;

      map_type env_vars_;
      map_type get_vars_;
      map_type post_vars_;
      map_type cookie_vars_;

      std::string null_str_;
      bool all_done_;

      mutable_buffers_type prepare(std::size_t size)
      {
        using namespace std;
        std::size_t bufsz( buffer_.size() );
        cerr<< "bufsz    = " << bufsz << endl;

        // Reserve more space if it's needed.
        // (this could be safer, referencing this against CONTENT_LENGTH)
        //if ( (bufsz + size) >= buf_.capacity() )
        //{
          buffer_.resize(bufsz + size);
        //}

        cerr<< "Pre-read buffer (size: " << buffer_.size() 
            << "|capacity: " << buffer_.capacity() << ") == {" << endl
            << std::string(buffer_.begin(), buffer_.end()) << endl
   //         << "-----end buffer-----" << endl
   //         << "-------buffer-------" << endl
  //          << std::string(&buf_[0], &buf_[buf_.size()]) << endl
            << "}" << endl;
            ;
        //return boost::asio::buffer(&(*(buf_.end())), size);
  //      return boost::asio::buffer(&(*(buf_.begin())) + bufsz, size);
        return boost::asio::buffer(&buffer_[bufsz], size);
      }


     /************** New stuff *****************/
      header_buffer_type header_buf_;
      boost::uint16_t id_;
    };

    typedef fcgi_request_service                      type;
    typedef ::cgi::fcgi::fcgi_request_service         full_type;
    typedef type::implementation_type::protocol_type  protocol_type;
    typedef type::implementation_type::map_type       map_type;

    fcgi_request_service(::cgi::io_service& ios)
      : detail::service_base<fcgi_request_service>(ios)
    {
    }

    ~fcgi_request_service()
    {
    }

    void construct(implementation_type& impl)
    {
      //std::cerr<< "request_service.hpp:83 Creating connection" << std::endl;
      impl.client_.set_connection(//new implementation_type::connection_type(this->io_service()));
        implementation_type::connection_type::create(this->io_service())
      );
      //std::cerr<< "conn.is_open() == " << impl.client_.is_open() << std::endl;
    }

    void destroy(implementation_type& impl)
    {
      //if (!impl.all_done_)
      //  detail::abort_impl(impl); // this function isn't implemented yet!
      //impl.set_state(aborted);
    }

    void shutdown_service()
    {
    }

    void set_service(implementation_type& impl
                    , implementation_type::protocol_service_type& ps)
    {
      impl.service_ = &ps;
    }

    bool is_open(implementation_type& impl)
    {
      return !impl.all_done_ && impl.client_.is_open();
    }

    /// Close the request.
    int close(implementation_type& impl, http::status_code& hsc
              , int program_status)
    {
      impl.all_done_ = true;
      impl.client_.close();
      return program_status;
    }

    /// Load the request to a point where it can be usefully used.
    /**
     * FastCGI:
     * --------
     *
     *  - Calls client::construct() to claim a request id from the server.
     *  - Reads, parses and handles all packets until the closing `PARAMS`
     *    packet for this request arrives from the server.
     *
     */
    boost::system::error_code&
      load(implementation_type& impl, bool parse_stdin
          , boost::system::error_code& ec)
    {
      //int header_len( get_length_of_header(impl, ec) );
      std::cerr<< "Loading request...";
      BOOST_ASSERT(!ec && "Can't load request due to previous errors.");

      impl.client_.construct(impl, ec);
      BOOST_ASSERT(impl.client_.connection_->is_open());

      for(;;)
      {
        if (read_header(impl, ec))
          break;
        int id(fcgi::spec::get_request_id(impl.header_buf_));
        if (id == fcgi::spec::null_request_id::value)
          handle_admin_request(impl);
        else
        if (fcgi::spec::get_type(impl.header_buf_) 
            == fcgi::spec::begin_request::value)
        {
          impl.id_ = id;
          impl.client_.request_id_ = id;
          if (read_header(impl, ec))
            break;
          impl.request_role_ = fcgi::spec::begin_request::get_role(impl.header_buf_);
          std::cerr<< "[hw] New request role: " << impl.request_role_
              << " (" << fcgi::spec::role_type::to_string(impl.header_buf_) << ")"
              << std::endl;
          impl.client_.keep_connection_
            = fcgi::spec::begin_request::get_flags(impl.header_buf_)
              & fcgi::spec::keep_connection;
          break;
        }else
          handle_other_request_header(impl);
      }

      client_status completion_condition
        = parse_stdin ? stdin_read : params_read;

      while(!ec 
        && impl.client_.status() < completion_condition
        && impl.request_status_ != loaded)
      {
        //impl.client_.parse_packet(impl, ec);
        parse_packet(impl, ec);
      }

      const std::string& request_method = env(impl, "REQUEST_METHOD", ec);
      if (request_method == "GET")
        if (parse_get_vars(impl, ec))
          return ec;
/** FIXME **
      else
      if (request_method == "POST" && parse_stdin)
        if (parse_post_vars(impl, ec))
	      return ec;
*/
      parse_cookie_vars(impl, ec);

      std::cerr<< "done!" << std::endl;
      return ec;
    }

    /* These Don't Belong Here.
    template<typename MutableBufferSequence>
    std::size_t read_some(implementation_type& impl
                         , const MutableBufferSequence& buf
                         , boost::system::error_code& ec)
    {
      std::size_t s = impl.connection()->read_some(buf, ec);
      return s;
    }

    template<typename ConstBufferSequence>
    std::size_t write_some(implementation_type& impl
                          , const ConstBufferSequence& buf
                          , boost::system::error_code& ec)
    {
      return impl.connection()->write_some(buf, ec);
    }

    //template<typename VarType> map_type& var(implementation_type&) const;
   ********************************************/

    std::string GET(implementation_type& impl, const std::string& name
                   , boost::system::error_code& ec)
    {
      return var(impl.get_vars_, name, ec);
    }

    map_type& GET(implementation_type& impl)
    {
      return impl.get_vars_;
    }

    /// Find the post meta-variable matching name
    /**
     * @param greedy This determines whether more data can be read to find
     * the variable. The default is true to cause least surprise in the common
     * case of having not parsed any of the POST data.

     -----------------------------------------------
     Should this return a pair of iterators instead?
     What about url_decoding?
     -----------------------------------------------

     */
    std::string POST(implementation_type& impl, const std::string& name
                    , boost::system::error_code& ec
                    , bool greedy = true)
    {
      const std::string& val = var(impl.post_vars_, name, ec);
      if (val.empty() && greedy && !ec)
      {

      }

      return val;
    }

    map_type& POST(implementation_type& impl)
    {
      return impl.post_vars_;
    }


    /// Find the cookie meta-variable matching name
    std::string cookie(implementation_type& impl, const std::string& name
                      , boost::system::error_code& ec)
    {
      return var(impl.cookie_vars_, name, ec);
    }

    map_type& cookie(implementation_type& impl)
    {
      return impl.cookie_vars_;
    }

    /// Find the environment meta-variable matching name
    std::string env(implementation_type& impl, const std::string& name
                   , boost::system::error_code& ec)
    {
      return var(impl.env_vars_, name, ec);
    }

    map_type& env(implementation_type& impl)
    {
      return impl.env_vars_;
    }

    role_type get_role(implementation_type& impl)
    {
      return responder;
    }

    implementation_type::client_type&
      client(implementation_type& impl)
    {
      return impl.client_;
    }

  protected:
    /// Extract the var value from 
    std::string var(map_type& _data, const std::string& _name
                   , boost::system::error_code& ec)
    {
      /* Alt:
      if ((typename map_type::iterator pos = meta_data.find(name))
             != meta_data.end())
      {
        return *pos;
      }
      return std::string();
      **/

      if( _data.find(_name) != _data.end() )
        return _data[_name];
      return "";
    }

    /// Read and parse the cgi GET meta variables
    boost::system::error_code&
    parse_get_vars(implementation_type& impl, boost::system::error_code& ec)
    {
      detail::extract_params(env(impl, "QUERY_STRING", ec)
                    , impl.get_vars_
                    , boost::char_separator<char>
                        ("", "=&", boost::keep_empty_tokens)
                    , ec);

      return ec;
    }

    /// Read and parse the HTTP_COOKIE meta variable
    boost::system::error_code&
    parse_cookie_vars(implementation_type& impl, boost::system::error_code& ec)
    {
      // Make sure this function hasn't already been called
      //BOOST_ASSERT( impl.cookie_vars_.empty() );

      std::string vars = env(impl, "HTTP_COOKIE", ec);
      if (vars.empty())
        return ec;

      detail::extract_params(env(impl, "HTTP_COOKIE", ec)
                            , impl.cookie_vars_
                            , boost::char_separator<char>
                                ("", "=&", boost::keep_empty_tokens)
                            , ec);

      return ec;
    }

    /// Read and parse the cgi POST meta variables (greedily)
    template<typename RequestImpl>
    boost::system::error_code&
    parse_post_vars(implementation_type& impl, boost::system::error_code& ec)
    {
      // Make sure this function hasn't already been called
      //BOOST_ASSERT( impl.post_vars_.empty() );
	  
      //#     error "Not implemented"

      if (impl.stdin_parsed_)
      {
      }

      return ec;
    }

    /// Read and parse a single cgi POST meta variable (greedily)
    template<typename RequestImpl>
    boost::system::error_code&
    parse_one_post_var(implementation_type& impl, boost::system::error_code& ec)
    {
      //#     error "Not implemented"
      return ec;
    }

  /***************************************************************************/
  public: // Reading stuff goes under here


    /// Read some data from the client.
    template<typename MutableBufferSequence>
    std::size_t
      read_some(implementation_type& impl, const MutableBufferSequence& buf
               , boost::system::error_code& ec)
    {
      if (impl.client_.status_ == closed_)
      {
        ec = error::client_closed;
        return 0;
      }

      //if (read_header(ec))
        return -1;

      //boost::tribool state = parse_header(impl);
      //std::size_t bytes_read;//( connection_->read_some(buf, ec) );
      //return bytes_read;
    }

    /// Read a single header, buf do nothing with it.
    boost::system::error_code
      read_header(implementation_type& impl, boost::system::error_code& ec)
    {
      // clear the header first (might be unneccesary).
      impl.header_buf_ = implementation_type::header_buffer_type();

      if (8 != read(*impl.client_.connection_, buffer(impl.header_buf_)
                   , boost::asio::transfer_all(), ec) || ec)
        return ec;
      
      //if (ec) return ec;

      std::cerr<< std::endl
          << "[hw] Header details {" << std::endl
          << "  RequestId := " << fcgi::spec::get_request_id(impl.header_buf_) << std::endl
          << "  FastCGI version := " << fcgi::spec::get_version(impl.header_buf_) << std::endl
          << "  Type := " << fcgi::spec::get_type(impl.header_buf_)
          << " (" << fcgi::spec::request_type::to_string(impl.header_buf_) << ")" << std::endl
          << "  Content-length := " << fcgi::spec::get_content_length(impl.header_buf_) << std::endl
          << "}" << std::endl;

      return ec;
    }

    /*** Various handlers go below here; they might find a
     * better place to live ***/

    // **FIXME**
    void handle_admin_request(implementation_type& impl)
    {
      std::cerr<< std::endl << "**FIXME** " << __FILE__ << ":" << __LINE__ 
        << " handle_admin_request()" << std::endl;
    }

    // **FIXME**
    void handle_other_request_header(implementation_type& impl)
    {
      std::cerr<< std::endl << "**FIXME** " << __FILE__ << ":" << __LINE__ 
        << " handle_other_request_header()" << std::endl;
    }

    // **FIXME**    
    boost::system::error_code
      process_begin_request(implementation_type& impl, boost::uint16_t id
                           , const unsigned char* buf, boost::uint16_t
                           , boost::system::error_code& ec)
    {
      //implementation_type& req
      //  = impl.client_.request_id_ == 0
      //    ? impl
      //    : get_or_make_request(impl);
          
      if (impl.client_.request_id_ == 0) // ie. not set
      {
        impl.client_.request_id_ = fcgi::spec::get_request_id(impl.header_buf_);

        BOOST_STATIC_ASSERT((
          fcgi::spec::begin_request::body::size::value
          == fcgi::spec::header_length::value));
        
        // A begin request body is as long as a header, so we can optimise:
        if (read_header(impl, ec))
          return ec;
         
        impl.request_role_ = fcgi::spec::begin_request::get_role(impl.header_buf_);
        std::cerr<< "[hw] New request role: " << impl.request_role_
            << " (" << fcgi::spec::role_type::to_string(impl.header_buf_) << ")"
            << std::endl;
        impl.client_.keep_connection_
          = fcgi::spec::begin_request::get_flags(impl.header_buf_)
            & fcgi::spec::keep_connection;

        impl.client_.status_ = constructed;
      }
      else
      {
        std::cerr<< "**FIXME** Role: " 
          << fcgi::spec::begin_request::get_role(impl.header_buf_) << std::endl;
        return error::multiplexed_request_incoming;
      }

      //connection_->request_map_[id] = 
      return ec;
    }

    boost::system::error_code
      process_abort_request(implementation_type& impl, boost::uint16_t id
                           , const unsigned char* buf, boost::uint16_t
                           , boost::system::error_code& ec)
    {
      if (id == fcgi::spec::get_request_id(impl.header_buf_))
      {
        impl.request_status_ = aborted;
        return ec;
      }
      try {
        std::cerr<< "**FIXME** request aborted (id = " << id
          << ") but request not notified." << std::endl;
        //impl.client_.connection_->requests_.at(id - 1)->abort();
      }catch(...){
        ec = error::abort_request_record_recieved_for_invalid_request;
      }
/*
      connection_type::request_map_type::iterator i
        = connection_->find(id);

      if (i == connection_type::request_map_type::iterator())
      {
        return bad_request_id;
      }

      //lookup_request(id).abort();
*/
      return ec;
    }

    boost::system::error_code
      process_params(implementation_type& impl, boost::uint16_t id
                    , const unsigned char* buf, boost::uint16_t len
                    , boost::system::error_code& ec)
    {
      if (0 == len)
      { // This is the final param record.
        
        impl.client_.status_ = params_read;

        std::cerr<< "[hw] Final PARAM record found." << std::endl;
        return ec;
      }

      while(len)
      {
        boost::uint32_t   name_len, data_len;
        std::string name, data;
        if (*buf >> 7 == 0)
        {
            name_len = *(buf++);
            --len;
        }
        else
        {
            name_len = ((buf[0] & 0x7F) << 24)
                     +  (buf[1]         << 16)
                     +  (buf[2]         <<  8)
                     +   buf[3];
            buf += 4;
            len -= 4;
        }

        if (*buf >> 7 == 0)
        {
            data_len = *(buf++);
            --len;
        }
        else
        {
            data_len = ((buf[0] & 0x7F) << 24)
                     +  (buf[1]         << 16)
                     +  (buf[2]         <<  8)
                     +   buf[3];
            buf += 4;
            len -= 4;
        }
        name.assign(reinterpret_cast<const char*>(buf), name_len);
        data.assign(reinterpret_cast<const char*>(buf)+name_len, data_len);
        buf += (name_len + data_len);
        len -= (name_len + data_len);

        std::cerr<< "[hw] name := " << name << std::endl;
        std::cerr<< "[hw] data := " << data << std::endl;

        impl.env_vars_[name] = data;
      }

      return ec;
    }


    boost::system::error_code
      process_stdin(implementation_type& impl, boost::uint16_t id
                   , const unsigned char* buf, boost::uint16_t len
                   , boost::system::error_code& ec)
    {
      if (0 == len)
      {
        impl.client_.status_ = stdin_read;

        // **FIXME**
        std::cerr<< "[hw] Final STDIN record found." << std::endl;
        return ec;
      }

      // **FIXME**
      std::cerr<< "[hw] Found some STDIN stuff." << std::endl;
      return ec;
    }

    /// Parse the current header
    boost::tribool parse_header(implementation_type& impl)
    {
      BOOST_ASSERT(fcgi::spec::get_version(impl.header_buf_) == 1
                   && "This library is only compatible with FastCGI 1.0");

      using namespace fcgi::spec_detail;

      switch(fcgi::spec::get_type(impl.header_buf_))
      {
      case BEGIN_REQUEST:
      case PARAMS:
      case STDIN:
      case DATA:
      case GET_VALUES:
        return boost::indeterminate;
      case ABORT_REQUEST:
        return false;
      case UNKNOWN_TYPE:
      default:
        return true;
      }
    }
    
    // Mammoth typedef corresponding to function signature of process_*
    // functions.
    typedef boost::system::error_code
      ( full_type::* proc_func_t)
      (implementation_type& impl, boost::uint16_t, const unsigned char*
      , boost::uint16_t, boost::system::error_code&);

    static const proc_func_t proc_funcs[];
  
    boost::system::error_code
      parse_packet(implementation_type& impl, boost::system::error_code& ec)
    {
      //current_request_ = &req;

      if (this->read_header(impl, ec))
        return ec;

      boost::tribool state = this->parse_header(impl);

      if (state)
      { // the header has been handled and all is ok; continue.
        return ec;
      }else
      if (!state)
      { // The header is confusing; something's wrong. Abort.
        std::cerr<< "Bad header received (this isn't implemented properly yet"
            << std::endl;
        return error::bad_header_type;
      }
      // else route (ie. state == boost::indeterminate)

      std::cerr<< "Got to read more stuff now I think." << std::endl;
      implementation_type::mutable_buffers_type buf
        = impl.prepare(fcgi::spec::get_length(impl.header_buf_));

      if (this->read_body(impl, buf, ec))
        return ec;

      this->parse_body(impl, buf, ec);

      return ec;
    }

    /// Read the body of the current packet; do nothing with it.
    template<typename MutableBuffersType>
    boost::system::error_code
      read_body(implementation_type& impl, const MutableBuffersType& buffer
               , boost::system::error_code& ec)
    {
      std::size_t bytes_read
        = read(*impl.client_.connection_, buffer, boost::asio::transfer_all(), ec);

      BOOST_ASSERT(bytes_read == fcgi::spec::get_length(impl.header_buf_)
                   && "Couldn't read all of the record body.");
      return ec;
    }

    template<typename MutableBuffersType>
    boost::system::error_code
      parse_body(implementation_type& impl, const MutableBuffersType& buffer
                , boost::system::error_code& ec)
    {
      return //ec;/*
        (this->* proc_funcs[fcgi::spec::get_type(impl.header_buf_)])
            (impl, fcgi::spec::get_request_id(impl.header_buf_)
            , boost::asio::buffer_cast<unsigned char*>(buffer)
            , boost::asio::buffer_size(buffer), ec);
    }

/*
    implementation_type&
      get_or_make_request(implementation_type& impl, boost::uint16_t id)
    {
      implementation_type::client_type::connection_type::request_vector_type&
        requests = impl.client_.connection_->requests;
      implementation_type* ret;
      
      try {
        ret = &requests.at(id - 1);
        BOOST_ASSERT(req != 0); // should throw
        return *ret;
      }catch(...){
        req = new request_type(impl.service_);
        if (requests.size() < (id - 1))
          requests.resize(id);
        requests.at(id-1) = *req;
        return *req; // same as *ret
      }
    }
*/

  };

  //template<>
  const fcgi_request_service::proc_func_t fcgi_request_service::proc_funcs[] =
    { 0
    , &fcgi_request_service::process_begin_request
    , &fcgi_request_service::process_abort_request
    , 0
    , &fcgi_request_service::process_params
    , &fcgi_request_service::process_stdin
    , 0
    , 0
    };

 } // namespace fcgi
} // namespace cgi

#endif // CGI_FCGI_REQUEST_SERVICE_HPP_INCLUDED__
