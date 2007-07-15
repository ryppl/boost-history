//              -- cgi_service_impl.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_CGI_SERVICE_IMPL_HPP_INCLUDED__
#define CGI_CGI_SERVICE_IMPL_HPP_INCLUDED__

#include "detail/extract_params.hpp"

namespace cgi {

  template<typename RequestImplType>
  class cgi_service_impl
  {
  public:
	typedef RequestImplType    impl_type;

    explicit cgi_service_impl()
    {
    }

    explicit cgi_service_impl(protocol_service_type&)
    {
    }

    void shutdown_service()
    {
    }

    void create(impl_type& impl)
    {
    }

    void destroy(impl_type& impl)
    {
    }

    /// Synchronously read/parse the request meta-data
    /**
     * @param parse_stdin if true then STDIN data is also read/parsed
     */
    boost::system::error_code
    load(boost::system::error_code& ec, bool parse_stdin)
    {
      std::string request_method = meta_get("REQUEST_METHOD");
      if (request_method == "GET")
        if (parse_get_vars(ec))
	        return ec;
      else
      if (request_method == "POST" && parse_stdin)
	      if (parse_post_vars(ec))
	        return ec;

      parse_cookie_vars(ec);
      return ec;
    }

    template<typename ConnectionPtr, typename MutableBufferSequence>
    std::size_t read_some(impl_type& impl, const MutableBufferSequence& buf
                         , boost::system::error_code& ec)
    {
      std::size_t s = impl.connection_->read_some(buf, ec);
      return s;
    }

    template<typename ConnectionPtr, typename ConstBufferSequence>
    std::size_t write_some(impl_type& impl, const ConstBufferSequence& buf
                          , boost::system::error_code& ec)
    {
      return impl.connection_->write_some(buf, ec);
    }

    //template<typename VarType> map_type& var(impl_type&) const;

    const std::string& var(map_type& meta_data, const std::string& name
                          , boost::system::error_code& ec) const
    {
      if ((map_type::iterator pos = meta_data.find(name)) != meta_data.end())
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

	const std::string& meta_get(impl_type& impl, const std::string& name
                               , boost::system::error_code& ec) const
    {
      return var(impl.get_vars_, name, ec);
    }

    map_type& meta_get(impl_type& impl) const
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
    const std::string& meta_post(impl_type& impl, const std::string& name
                                , boost::system::error_code& ec
                                , bool greedy = true)
    {
      const std::string& val = var(impl.post_vars_, name, ec);
      if (var.empty() && greedy && !ec)
      {

      }

      return val;
    }

    map_type& meta_post(impl_type& impl) const
    {
      return impl.post_vars_;
    }


    /// Find the cookie meta-variable matching name
    const std::string& meta_cookie(impl_type& impl, const std::string& name
                                  , boost::system::error_code& ec) const
    {
      return var(impl.cookie_vars_, name, ec, false);
    }

    map_type& meta_cookie(impl_type& impl) const
    {
      return impl.cookie_vars_;
    }


    /// Find the environment meta-variable matching name
    const std::string& meta_env(impl_type& impl, const std::string& name,
                               , boost::system::error_code& ec) const
    {
	  return ::getenv(name);
    }


    role_type get_role(impl_type& impl) const
    {
      return role_type::responder;
    }

  protected:
    /// Read and parse the cgi GET meta variables
    boost::system::error_code&
    parse_get_vars(impl_type& impl, boost::system::error_code& ec)
    {
      detail::extract_params(meta_env("QUERY_STRING")
                            , impl.get_vars_
                            , boost::char_separator<char>
                                ("", "=&", boost::keep_empty_tokens)
                            , ec);

      return ec;
    }

    /// Read and parse the HTTP_COOKIE meta variable
    boost::system::error_code
    parse_cookie_vars(impl_type& impl, boost::system::error_code& ec)
    {
      // Make sure this function hasn't already been called
      BOOST_ASSERT( impl.cookie_vars_.empty() );

      std::string vars = meta_env("HTTP_COOKIE");
      if (vars.empty())
        return ec;

      detail::extract_params(meta_env("HTTP_COOKIE")
                            , impl.cookie_vars_
                            , boost::char_separator<char>
                                ("", "=&", boost::keep_empty_tokens)
                            , ec);

      return ec;
    }

    /// Read and parse the cgi POST meta variables (greedily)
    boost::system::error_code
    parse_post_vars(impl_type& impl, boost::system::error_code& ec)
    {
      // Make sure this function hasn't already been called
      BOOST_ASSERT( impl.cookie_vars_.empty() );
	  
#     error "Not implemented"

      if (impl_.stdin_parsed_)
      {
      }

      return ec;
    }

    /// Read and parse a single cgi POST meta variable (greedily)
    boost::system::error_code
    parse_one_post_var(impl_type& impl, boost::system::error_code& ec)
    {
#     error "Not implemented"
      return ec;
    }
  };

} // namespace cgi

#endif // CGI_CGI_SERVICE_IMPL_HPP_INCLUDED__
