//               -- scgi/service_impl.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_SCGI_SERVICE_IMPL_HPP_INCLUDED__
#define CGI_SCGI_SERVICE_IMPL_HPP_INCLUDED__

/*********************

THIS IS NOT A PART OF 'THE CODEBASE' ATM

*********************/

#include <boost/system/error_code.hpp>

#include "boost/cgi/scgi/request_impl.hpp"
#include "boost/cgi/map.hpp"
#include "boost/cgi/tags.hpp"
#include "boost/cgi/role_type.hpp"
#include "boost/cgi/io_service.hpp"
#include "boost/cgi/detail/throw_error.hpp"
#include "boost/cgi/detail/service_base.hpp"
#include "boost/cgi/detail/extract_params.hpp"

namespace cgi {

  // The implementation of the ProtocolService for SCGI
  class scgi_service_impl
    : public detail::service_base<scgi_service_impl>
  {
  public:
    typedef tags::scgi          protocol_type;
    typedef cgi::map            map_type;

    scgi_request_service_impl(cgi::io_service& ios)
      : detail::service_base<scgi_request_service_impl>(ios)
      , io_service_(ios)
    {
    }

    ~scgi_request_service_impl()
    {
    }

  };

} // namespace cgi

#endif // CGI_SCGI_SERVICE_IMPL_HPP_INCLUDED__
