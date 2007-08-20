#ifndef CGI_CGI_SERVICE_IMPL_BASE_HPP_INCLUDED__
#define CGI_CGI_SERVICE_IMPL_BASE_HPP_INCLUDED__

#include "../detail/push_options.hpp"

#include <string>
#include <cstdlib>
#include <iostream>
#include <boost/assert.hpp>
#include <boost/tokenizer.hpp>
#include <boost/system/error_code.hpp>

#include "boost/cgi/map.hpp"
#include "boost/cgi/role_type.hpp"
#include "boost/cgi/detail/extract_params.hpp"
//#include "../connections/stdio.hpp"

namespace cgi {

 namespace detail {


 } // namespace detail

  using std::cerr;
  using std::endl;


  template<typename RequestImplType>
  class cgi_service_impl_base
  {
  public:
    typedef RequestImplType    implementation_type;
    typedef cgi::map           map_type;

    cgi_service_impl_base()
    {
    }

    template<typename T>
    cgi_service_impl_base(T&)
    {
    }

    bool is_open(implementation_type& impl)
    {
      return true;
    }

    int close(implementation_type& impl, http::status_code&, int status)
    {
      //impl.set_status(aborted);
      return status;
    }

    /// Synchronously read/parse the request meta-data
    /**
     * @param parse_stdin if true then STDIN data is also read/parsed
     */
    boost::system::error_code&
    load(implementation_type& impl, bool parse_stdin
        , boost::system::error_code& ec)
    {
      const std::string& request_method = meta_env(impl, "REQUEST_METHOD", ec);
      if (request_method == "GET")
        if (parse_get_vars(impl, ec))
	      return ec;
      else
      if (request_method == "POST" && parse_stdin)
        if (parse_post_vars(impl, ec))
	      return ec;

      parse_cookie_vars(impl, ec);
      return ec;
    }

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

    std::string var(map_type& meta_data, const std::string& name
                   , boost::system::error_code& ec)
    {
      /* Alt:
      if ((typename map_type::iterator pos = meta_data.find(name))
             != meta_data.end())
      {
        return pos->second;
      }
      return std::string();
      *********
      for(typename map_type::iterator iter = meta_data.begin()
         ; iter != meta_data.end()
         ; ++iter)
      {
        if( iter->first == name )
          return iter->second;
      }
      return "";
      **/

      if( meta_data.find(name) != meta_data.end() )
        return meta_data[name];
      return "";
    }

	std::string meta_get(implementation_type& impl, const std::string& name
                        , boost::system::error_code& ec)
    {
      return var(impl.get_vars(), name, ec);
    }

    map_type& meta_get(implementation_type& impl)
    {
      return impl.get_vars();
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
    std::string meta_post(implementation_type& impl, const std::string& name
                         , boost::system::error_code& ec
                         , bool greedy = true)
    {
      const std::string& val = var(impl.post_vars(), name, ec);
      if (val.empty() && greedy && !ec)
      {

      }

      return val;
    }

    map_type& meta_post(implementation_type& impl)
    {
      return impl.post_vars();
    }


    /// Find the cookie meta-variable matching name
    std::string cookie(implementation_type& impl, const std::string& name
                      , boost::system::error_code& ec)
    {
      return var(impl.cookie_vars(), name, ec);
    }

    map_type& meta_cookie(implementation_type& impl)
    {
      return impl.cookie_vars();
    }


    /// Find the environment meta-variable matching name
    std::string meta_env(implementation_type& impl, const std::string& name
                        , boost::system::error_code& ec)
    {
      const char* c = ::getenv(name.c_str());
      return c ? c : impl.null_str();
    }


    role_type get_role(implementation_type& impl)
    {
      return responder;
    }

  protected:
    /// Read and parse the cgi GET meta variables
    template<typename RequestImpl>
    boost::system::error_code&
    parse_get_vars(RequestImpl& impl, boost::system::error_code& ec)
    {
      detail::extract_params(meta_env(impl, "QUERY_STRING", ec)
                    , impl.get_vars()
                    , boost::char_separator<char>
                        ("", "=&", boost::keep_empty_tokens)
                    , ec);

      return ec;
    }

    /// Read and parse the HTTP_COOKIE meta variable
    template<typename RequestImpl>
    boost::system::error_code&
    parse_cookie_vars(RequestImpl& impl, boost::system::error_code& ec)
    {
      // Make sure this function hasn't already been called
      //BOOST_ASSERT( impl.cookie_vars_.empty() );

      std::string vars = meta_env(impl, "HTTP_COOKIE", ec);
      if (vars.empty())
        return ec;

      detail::extract_params(meta_env(impl, "HTTP_COOKIE", ec)
                            , impl.cookie_vars()
                            , boost::char_separator<char>
                                ("", "=&", boost::keep_empty_tokens)
                            , ec);

      return ec;
    }

    /// Read and parse the cgi POST meta variables (greedily)
    template<typename RequestImpl>
    boost::system::error_code&
    parse_post_vars(RequestImpl& impl, boost::system::error_code& ec)
    {
      // Make sure this function hasn't already been called
      //BOOST_ASSERT( impl.post_vars_.empty() );
	  
      //#     error "Not implemented"

      if (impl.stdin_parsed())
      {
      }

      return ec;
    }

    /// Read and parse a single cgi POST meta variable (greedily)
    template<typename RequestImpl>
    boost::system::error_code&
    parse_one_post_var(RequestImpl& impl, boost::system::error_code& ec)
    {
      //#     error "Not implemented"
      return ec;
    }
  };

} // namespace cgi

#include "../detail/pop_options.hpp"

#endif // CGI_CGI_SERVICE_IMPL_HPP_INCLUDED__
