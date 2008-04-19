//                   -- form_parser.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_DETAIL_FORM_PARSER_HPP_INCLUDED__
#define CGI_DETAIL_FORM_PARSER_HPP_INCLUDED__

#include "boost/cgi/detail/push_options.hpp"

#include <set>
#include <vector>
#include <string>
#include <boost/regex.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/error.hpp>
#include <boost/function.hpp>
#include <boost/system/error_code.hpp>
#include "boost/cgi/common/form_part.hpp"
#include "boost/cgi/basic_client.hpp"

namespace cgi {
 namespace detail {

  /// Destined for better things than an implementation detail (hopefully).
  template<typename RequestImplType>
  class form_parser
  {
  public:
    //typedef
    //  boost::function<
    //    std::size_t (
    //        const mutable_buffers_type&
    //      , boost::system::error_code& )
    //  >
    //callback_type;

    typedef boost::asio::mutable_buffers_1 mutable_buffers_type;
    typedef std::vector<char>   buffer_type;
    typedef ::cgi::common::map  map_type;

    typedef RequestImplType     implementation_type;

    form_parser(implementation_type& impl);

    mutable_buffers_type prepare(std::size_t size)
    {
      std::size_t bufsz(impl_.buf_.size());
      impl_.buf_.resize(bufsz + size);
      return boost::asio::buffer(&impl_.buf_[bufsz], size);
    }
      
    std::string buffer_string()
    {
      return std::string(impl_.buf_.begin() + offset_, impl_.buf_.end());
    }
    
    boost::system::error_code 
      parse(boost::system::error_code& ec);

    boost::system::error_code
      parse_url_encoded_form(boost::system::error_code& ec);

    boost::system::error_code
      parse_multipart_form(boost::system::error_code& ec);

    boost::system::error_code
      parse_form_part(boost::system::error_code& ec);

    boost::system::error_code
      parse_form_part_data(boost::system::error_code& ec);

    boost::system::error_code
      parse_form_part_meta_data(boost::system::error_code& ec);

    boost::system::error_code
      move_to_start_of_first_part(boost::system::error_code& ec);

    /// Get the boundary marker from the CONTENT_TYPE header.
    boost::system::error_code
      parse_boundary_marker(boost::system::error_code& ec);

  private:
    implementation_type& impl_;
    std::size_t& bytes_left_;
    buffer_type::iterator pos_;
    //bool& stdin_data_read_;
    std::size_t offset_;

    std::string boundary_marker;
    std::list<std::string> boundary_markers;
    std::vector<common::form_part> form_parts_;

    //callback_type callback_;
  };

 } // namespace detail
} // namespace cgi

#include "boost/cgi/common/form_parser.ipp"

#endif // CGI_DETAIL_FORM_PARSER_HPP_INCLUDED__

